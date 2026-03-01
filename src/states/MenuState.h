#pragma once
#include "states/IGameState.h"
#include "core/Logger.h"

// √лавное меню Ч нажми Enter чтобы начать игру
class MenuState : public IGameState {
public:
    void onEnter() override;
    void onExit() override;
    void update(float dt) override;
    void render() override;

    bool shouldStartGame() const { return startGame_; }

private:
    bool startGame_ = false;
};