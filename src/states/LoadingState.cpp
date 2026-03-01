#include "states/LoadingState.h"

void LoadingState::onEnter() {
    LOG_INFO("LoadingState: entered");
    timer_ = 0.0f;
    finished_ = false;
}

void LoadingState::onExit() {
    LOG_INFO("LoadingState: exited");
}

void LoadingState::update(float dt) {
    timer_ += dt; // накапливаем время

    if (timer_ >= duration_) {
        finished_ = true; // прошло 2 секунды - готово
    }
}

void LoadingState::render() {
    // Пока просто тёмный экран — треугольник добавим позже
}