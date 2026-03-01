#pragma once
#include <glad/glad.h>
#include <string>
#include "core/Logger.h"

// Загружает шейдеры из файлов и компилирует их на видеокарте
class ShaderProgram {
public:
    // Загрузить и скомпилировать шейдеры
    bool load(const std::string& vertexPath, const std::string& fragmentPath);

    // Сказать OpenGL использовать эту программу для рисования
    void use() const;

    // Удалить программу с видеокарты
    void destroy();

	GLuint getId() const { return id_; } // получить ID программы (для установки uniform-переменных и т.п.)

private:
    GLuint id_ = 0; // ID программы на видеокарте

    // Прочитать файл в строку
    std::string readFile(const std::string& path);

    // Скомпилировать один шейдер
    GLuint compileShader(GLenum type, const std::string& source);
};