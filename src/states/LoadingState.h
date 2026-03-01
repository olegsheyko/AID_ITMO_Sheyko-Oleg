#pragma once
#include "states/IGameState.h"
#include "core/Logger.h"

// Состояние загрузки — первое что видит игрок.
// Просто ждём немного и переходим в меню.
class LoadingState : public IGameState {
public:
    void onEnter() override;
    void onExit() override;
    void update(float dt) override;
    void render() override;

    // Application проверяет этот флаг чтобы переключиться в меню
    bool isFinished() const { return finished_; }

private:
    float timer_ = 0.0f;         // сколько времени прошло
    const float duration_ = 2.0f; // сколько секунд показываем загрузку
    bool finished_ = false;
};