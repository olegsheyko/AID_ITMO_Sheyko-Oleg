#pragma once
#include "states/IGameState.h"
#include "core/Logger.h"
#include "render/ShaderProgram.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// ќсновное игровое состо€ние
class GameplayState : public IGameState {
public:
    void onEnter() override;
    void onExit() override;
    void update(float dt) override;
    void render() override;

private:
	ShaderProgram shader_; // шейдер дл€ рисовани€ треугольника

	GLuint VAO_ = 0; // Vertex Array Object дл€ треугольника
	GLuint VBO_ = 0; // Vertex Buffer Object дл€ треугольника

    float x_ = 0.0f; // позици€ объекта Ч пригодитс€ когда добавим треугольник
    float y_ = 0.0f;

	float centerX_ = 0.0f; // центр треугольника по X
	float centerY_ = 0.0f; // центр треугольника по Y

	float scale_ = 1.0f; // масштаб объекта
	float rotation_ = 0.0f; // угол поворота объекта

	bool rmbWasPressed_ = false; // была ли нажата права€ кнопка мыши
	bool lmbWasPressed_ = false; // была ли нажата лева€ кнопка мыши
	bool mmbWasPressed_ = false; // была ли нажата средн€€ кнопка мыши
};