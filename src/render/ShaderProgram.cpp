#include "render/ShaderProgram.h"
#include <fstream>
#include <sstream>

bool ShaderProgram::load(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertSrc = readFile(vertexPath);
    std::string fragSrc = readFile(fragmentPath);

    if (vertSrc.empty() || fragSrc.empty()) {
        LOG_ERROR("ShaderProgram: failed to read shader files");
        return false;
    }

    // Компилируем оба шейдера
    GLuint vert = compileShader(GL_VERTEX_SHADER, vertSrc);
    GLuint frag = compileShader(GL_FRAGMENT_SHADER, fragSrc);

    if (!vert || !frag) return false;

    // Линкуем их в одну программу
    id_ = glCreateProgram();
    glAttachShader(id_, vert);
    glAttachShader(id_, frag);
    glLinkProgram(id_);

    // Проверяем ошибки линковки
    int success;
    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(id_, 512, nullptr, log);
        LOG_ERROR("ShaderProgram: link error: " + std::string(log));
        return false;
    }

    // Отдельные шейдеры больше не нужны — они уже внутри программы
    glDeleteShader(vert);
    glDeleteShader(frag);

    LOG_INFO("ShaderProgram: compiled OK");
    return true;
}

void ShaderProgram::use() const {
    glUseProgram(id_);
}

void ShaderProgram::destroy() {
    glDeleteProgram(id_);
}

std::string ShaderProgram::readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        LOG_ERROR("ShaderProgram: cannot open file: " + path);
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

GLuint ShaderProgram::compileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // Проверяем ошибки компиляции
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        LOG_ERROR("ShaderProgram: compile error: " + std::string(log));
        return 0;
    }
    return shader;
}