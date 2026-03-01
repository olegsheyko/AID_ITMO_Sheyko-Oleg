#pragma once
#include "states/IGameState.h"
#include <memory>
#include <stack>

class StateManager {
public:
	void push(std::unique_ptr<IGameState> state);

	void pop();

	void change(std::unique_ptr<IGameState> state);

	IGameState* current();

	bool isEmpty() const;

private:
	std::stack<std::unique_ptr<IGameState>> states_;
};