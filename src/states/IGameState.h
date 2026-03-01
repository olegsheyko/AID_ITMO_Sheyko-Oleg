#pragma once

class IGameState {
public:
	virtual ~IGameState() = default;

	virtual void onEnter() {}

	virtual void onExit() {}

	virtual void update(float dt) = 0;

	virtual void render() = 0;
};