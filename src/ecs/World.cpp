#include "ecs/World.h"
#include "ecs/Components.h"

#include <algorithm>

Entity World::createEntity() {
    const Entity entity = nextEntity_++;
    aliveEntities_.insert(entity);
    return entity;
}

void World::destroyEntity(Entity entity) {
    if (!isAlive(entity)) {
        return;
    }

    if (hasComponent<Hierarchy>(entity)) {
        const Hierarchy hierarchy = getComponent<Hierarchy>(entity);

        if (hierarchy.parent != kInvalidEntity && isAlive(hierarchy.parent) && hasComponent<Hierarchy>(hierarchy.parent)) {
            auto& siblings = getComponent<Hierarchy>(hierarchy.parent).children;
            siblings.erase(std::remove(siblings.begin(), siblings.end(), entity), siblings.end());
        }

        for (Entity child : hierarchy.children) {
            if (isAlive(child) && hasComponent<Hierarchy>(child)) {
                getComponent<Hierarchy>(child).parent = kInvalidEntity;
            }
        }
    }

    aliveEntities_.erase(entity);

    for (auto& [type, storage] : storages_) {
        (void)type;
        storage->remove(entity);
    }
}

void World::clear() {
    aliveEntities_.clear();
    storages_.clear();
    updateSystems_.clear();
    renderSystems_.clear();
    nextEntity_ = 1;
}

bool World::isAlive(Entity entity) const {
    return aliveEntities_.find(entity) != aliveEntities_.end();
}

void World::addUpdateSystem(UpdateSystem& system) {
    if (std::find(updateSystems_.begin(), updateSystems_.end(), &system) == updateSystems_.end()) {
        updateSystems_.push_back(&system);
    }
}

void World::addRenderSystem(RenderSystemBase& system) {
    if (std::find(renderSystems_.begin(), renderSystems_.end(), &system) == renderSystems_.end()) {
        renderSystems_.push_back(&system);
    }
}

void World::updateSystems(float dt) {
    for (UpdateSystem* system : updateSystems_) {
        if (system != nullptr) {
            system->update(*this, dt);
        }
    }
}

void World::renderSystems() {
    for (RenderSystemBase* system : renderSystems_) {
        if (system != nullptr) {
            system->render(*this);
        }
    }
}
