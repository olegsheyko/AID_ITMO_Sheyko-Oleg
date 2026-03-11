#include "ecs/RenderSystem.h"
#include "ecs/Components.h"
#include "ecs/World.h"
#include "math/MathTypes.h"
#include "render/IRenderAdapter.h"

#include <unordered_set>

namespace {
Mat4 buildLocalMatrix(const Transform& transform) {
    return Math::composeTransform(transform.position, transform.rotation, transform.scale);
}

Mat4 buildWorldMatrix(World& world, Entity entity, std::unordered_set<Entity>& visited) {
    if (!world.isAlive(entity) || !world.hasComponent<Transform>(entity)) {
        return Mat4::identity();
    }

    if (!visited.insert(entity).second) {
        return buildLocalMatrix(world.getComponent<Transform>(entity));
    }

    Mat4 worldMatrix = buildLocalMatrix(world.getComponent<Transform>(entity));

    if (world.hasComponent<Hierarchy>(entity)) {
        const Hierarchy& hierarchy = world.getComponent<Hierarchy>(entity);
        if (hierarchy.parent != kInvalidEntity) {
            const Mat4 parentMatrix = buildWorldMatrix(world, hierarchy.parent, visited);
            worldMatrix = Math::multiply(parentMatrix, worldMatrix);
        }
    }

    visited.erase(entity);
    return worldMatrix;
}
}

RenderSystem::RenderSystem(IRenderAdapter& renderer)
    : renderer_(renderer) {
}

void RenderSystem::render(World& world) {
    world.forEach<Transform, MeshRenderer>([this, &world](Entity entity, Transform& transform, MeshRenderer& renderer) {
        (void)transform;
        std::unordered_set<Entity> visited;
        const Mat4 modelMatrix = buildWorldMatrix(world, entity, visited);
        this->renderer_.drawPrimitive(renderer.primitive, modelMatrix, renderer.color);
    });
}
