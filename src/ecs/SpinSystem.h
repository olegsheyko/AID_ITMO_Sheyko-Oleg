#pragma once

#include "ecs/System.h"

class SpinSystem : public UpdateSystem {
public:
    void update(World& world, float dt) override;
};
