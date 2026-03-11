#include "ecs/World.h"

Entity World::createEntity() {
    const Entity entity = nextEntity_++;
    aliveEntities_.insert(entity);
    return entity;
}

void World::destroyEntity(Entity entity) {
    if (!isAlive(entity)) {
        return;
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
    nextEntity_ = 1;
}

bool World::isAlive(Entity entity) const {
    return aliveEntities_.find(entity) != aliveEntities_.end();
}
