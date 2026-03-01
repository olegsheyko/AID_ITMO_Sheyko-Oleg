#include "states/StateManager.h"
#include "core/Logger.h"

void StateManager::push(std::unique_ptr<IGameState> state) {
	state->onEnter();
	states_.push(std::move(state));
	LOG_INFO("StateManager: Pushed new state, total states: " + std::to_string(states_.size()));
}

void StateManager::pop() {
	if (!states_.empty()) {
		states_.top()->onExit();
		states_.pop();
		LOG_INFO("StateManager: Popped state, total states: " + std::to_string(states_.size()));
	}
}

void StateManager::change(std::unique_ptr<IGameState> state) {
	if (!states_.empty()) {
		states_.top()->onExit();
		states_.pop();
	}
	state->onEnter();
	states_.push(std::move(state));
	LOG_INFO("StateManager: Changed state, total states: " + std::to_string(states_.size()));
}

IGameState* StateManager::current() {
	if (states_.empty()) return nullptr;
	return states_.top().get();
}

bool StateManager::isEmpty() const {
	return states_.empty();
}