#include "states/MenuState.h"
#include <GLFW/glfw3.h>

void MenuState::onEnter() {
    LOG_INFO("MenuState: entered");
    startGame_ = false;
}

void MenuState::onExit() {
    LOG_INFO("MenuState: exited");
}

void MenuState::update(float dt) {
    (void)dt;
    // Проверяем нажатие Enter
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ENTER) == GLFW_PRESS) {
        startGame_ = true;
    }
}

void MenuState::render() {
    // Пока пустой экран — потом добавим треугольник
}