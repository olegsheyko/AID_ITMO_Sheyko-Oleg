#pragma once

class World;

class System {
public:
    virtual ~System() = default;
};

class UpdateSystem : public System {
public:
    virtual void update(World& world, float dt) = 0;
};
