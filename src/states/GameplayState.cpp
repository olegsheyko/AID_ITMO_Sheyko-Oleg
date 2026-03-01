#include "states/GameplayState.h"
#include <GLFW/glfw3.h>
#include <cmath>

void GameplayState::onEnter() {
    LOG_INFO("GameplayState: entered");

    shader_.load("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,  // левый нижний
         0.5f, -0.5f, 0.0f,  // правый нижний
         0.0f,  0.5f, 0.0f   // верхний центр
    };

	centerX_ = (vertices[0] + vertices[3] + vertices[6]) / 3.0f; // среднее X
	centerY_ = (vertices[1] + vertices[4] + vertices[7]) / 3.0f; // среднее Y

	glGenVertexArrays(1, &VAO_);
	glGenBuffers(1, &VBO_);

	glBindVertexArray(VAO_);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); // отщелкиваем VAO, чтобы случайно не изменить его настройки в другом месте
}

void GameplayState::onExit() {
    LOG_INFO("GameplayState: exited");
	glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
	shader_.destroy();
}

void GameplayState::update(float dt) {
    // Движение стрелками — пригодится когда добавим треугольник
    float speed = 1.0f * dt;

    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT) == GLFW_PRESS)  x_ -= speed;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT) == GLFW_PRESS) x_ += speed;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP) == GLFW_PRESS)    y_ += speed;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_DOWN) == GLFW_PRESS)  y_ -= speed;

    // LMB input
	bool lmbNow = glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    if (lmbNow && !lmbWasPressed_) {
        scale_ += 0.1f; // увеличиваем масштаб при каждом новом нажатии ЛКМ
	}
	lmbWasPressed_ = lmbNow;

	// RMB input
    bool rmbNow = glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    if (rmbNow && !rmbWasPressed_) {
        rotation_ += 3.14159f / 4.0f; // поворачиваем на 15 градусов при каждом новом нажатии ПКМ
	}
	rmbWasPressed_ = rmbNow;

	// MMB input
	bool mmbNow = glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;
	if (mmbNow && !mmbWasPressed_) {
		scale_ -= 0.1f; // сбрасываем масштаб при каждом новом нажатии СКМ
	}
	mmbWasPressed_ = mmbNow;
}

void GameplayState::render() {
    shader_.use();

	GLint posLoc = glGetUniformLocation(shader_.getId(), "uPosition");
	GLint scaleLoc = glGetUniformLocation(shader_.getId(), "uScale");
	GLint rotLoc = glGetUniformLocation(shader_.getId(), "uRotation");
	GLint centerLoc = glGetUniformLocation(shader_.getId(), "uCenter");

	glUniform2f(posLoc, x_, y_);
	glUniform1f(scaleLoc, scale_);
	glUniform1f(rotLoc, rotation_);
	glUniform2f(centerLoc, centerX_, centerY_);

    glBindVertexArray(VAO_);
    glDrawArrays(GL_TRIANGLES, 0, 3); // рисуем 3 вершины = 1 треугольник
}