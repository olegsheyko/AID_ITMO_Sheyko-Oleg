#include "states/GameplayState.h"
#include "ecs/Components.h"
#include "render/IRenderAdapter.h"

#include <GLFW/glfw3.h>
#include <algorithm>

namespace {
constexpr float kMoveSpeed = 1.0f;
constexpr float kScaleStep = 0.1f;
constexpr float kRotationStep = 3.1415926f / 4.0f;
constexpr float kMinScale = 0.1f;
}

GameplayState::GameplayState(IRenderAdapter& renderer)
	: renderSystem_(renderer) {
}

void GameplayState::onEnter() {
	LOG_INFO("GameplayState: entered");
	world_.clear();
	createScene();
	lmbWasPressed_ = false;
	rmbWasPressed_ = false;
	mmbWasPressed_ = false;
}

void GameplayState::onExit() {
	LOG_INFO("GameplayState: exited");
	world_.clear();
	controllableEntity_ = kInvalidEntity;
}

void GameplayState::update(float dt) {
	handleInput(dt);
	spinSystem_.update(world_, dt);
}

void GameplayState::render() {
	renderSystem_.render(world_);
}

void GameplayState::createScene() {
	controllableEntity_ = world_.createEntity();
	world_.addComponent<Tag>(controllableEntity_, "PlayerGroupRoot");
	world_.addComponent<Transform>(controllableEntity_, Transform{{-0.50f, -0.05f, 0.0f}, 0.0f, {0.32f, 0.22f, 1.0f}});
	world_.addComponent<MeshRenderer>(controllableEntity_, MeshRenderer{PrimitiveType::Quad, {0.95f, 0.35f, 0.28f, 1.0f}});
	world_.addComponent<Hierarchy>(controllableEntity_, Hierarchy{});

	const Entity wing = world_.createEntity();
	world_.addComponent<Tag>(wing, "PlayerWing");
	world_.addComponent<Transform>(wing, Transform{{0.52f, 0.00f, 0.0f}, 0.35f, {0.24f, 0.10f, 1.0f}});
	world_.addComponent<MeshRenderer>(wing, MeshRenderer{PrimitiveType::Quad, {1.00f, 0.76f, 0.32f, 1.0f}});
	attachChild(controllableEntity_, wing);

	const Entity wingTip = world_.createEntity();
	world_.addComponent<Tag>(wingTip, "PlayerWingTip");
	world_.addComponent<Transform>(wingTip, Transform{{0.72f, 0.00f, 0.0f}, 0.15f, {0.16f, 0.16f, 1.0f}});
	world_.addComponent<MeshRenderer>(wingTip, MeshRenderer{PrimitiveType::Triangle, {1.00f, 0.95f, 0.65f, 1.0f}});
	attachChild(wing, wingTip);

	const Entity beacon = world_.createEntity();
	world_.addComponent<Tag>(beacon, "PlayerBeacon");
	world_.addComponent<Transform>(beacon, Transform{{0.00f, 0.62f, 0.0f}, 0.0f, {0.20f, 0.20f, 1.0f}});
	world_.addComponent<MeshRenderer>(beacon, MeshRenderer{PrimitiveType::Triangle, {0.96f, 0.94f, 0.40f, 1.0f}});
	world_.addComponent<Spin>(beacon, Spin{2.20f});
	attachChild(controllableEntity_, beacon);

	const Entity rotatingQuad = world_.createEntity();
	world_.addComponent<Tag>(rotatingQuad, "RotatingQuad");
	world_.addComponent<Transform>(rotatingQuad, Transform{{0.20f, 0.42f, 0.0f}, 0.0f, {0.30f, 0.30f, 1.0f}});
	world_.addComponent<MeshRenderer>(rotatingQuad, MeshRenderer{PrimitiveType::Quad, {0.20f, 0.70f, 0.95f, 1.0f}});
	world_.addComponent<Spin>(rotatingQuad, Spin{1.35f});

	const Entity rotatingTriangle = world_.createEntity();
	world_.addComponent<Tag>(rotatingTriangle, "BeaconTriangle");
	world_.addComponent<Transform>(rotatingTriangle, Transform{{0.68f, 0.05f, 0.0f}, -0.35f, {0.18f, 0.18f, 1.0f}});
	world_.addComponent<MeshRenderer>(rotatingTriangle, MeshRenderer{PrimitiveType::Triangle, {1.00f, 0.88f, 0.30f, 1.0f}});
	world_.addComponent<Spin>(rotatingTriangle, Spin{-2.10f});

	const Entity ground = world_.createEntity();
	world_.addComponent<Tag>(ground, "Ground");
	world_.addComponent<Transform>(ground, Transform{{0.00f, -0.72f, 0.0f}, 0.0f, {1.45f, 0.14f, 1.0f}});
	world_.addComponent<MeshRenderer>(ground, MeshRenderer{PrimitiveType::Quad, {0.25f, 0.70f, 0.35f, 1.0f}});

	LOG_INFO("GameplayState: created ECS scene with hierarchy and 7 entities");
}

void GameplayState::handleInput(float dt) {
	if (!world_.isAlive(controllableEntity_)) {
		return;
	}

	GLFWwindow* window = glfwGetCurrentContext();
	if (window == nullptr) {
		return;
	}

	Transform& transform = world_.getComponent<Transform>(controllableEntity_);
	const float moveStep = kMoveSpeed * dt;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		transform.position.x -= moveStep;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		transform.position.x += moveStep;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		transform.position.y += moveStep;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		transform.position.y -= moveStep;
	}

	const bool lmbNow = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
	if (lmbNow && !lmbWasPressed_) {
		transform.scale.x += kScaleStep;
		transform.scale.y += kScaleStep;
	}
	lmbWasPressed_ = lmbNow;

	const bool rmbNow = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
	if (rmbNow && !rmbWasPressed_) {
		transform.rotation += kRotationStep;
	}
	rmbWasPressed_ = rmbNow;

	const bool mmbNow = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;
	if (mmbNow && !mmbWasPressed_) {
		transform.scale.x = std::max(kMinScale, transform.scale.x - kScaleStep);
		transform.scale.y = std::max(kMinScale, transform.scale.y - kScaleStep);
	}
	mmbWasPressed_ = mmbNow;
}

void GameplayState::attachChild(Entity parent, Entity child) {
	if (!world_.hasComponent<Hierarchy>(parent)) {
		world_.addComponent<Hierarchy>(parent, Hierarchy{});
	}

	Hierarchy childHierarchy{};
	childHierarchy.parent = parent;
	world_.addComponent<Hierarchy>(child, childHierarchy);

	world_.getComponent<Hierarchy>(parent).children.push_back(child);
}
