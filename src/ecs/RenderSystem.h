#pragma once

#include "ecs/System.h"

class IRenderAdapter;
class World;

class RenderSystem : public System {
public:
    explicit RenderSystem(IRenderAdapter& renderer);

    void render(World& world);

private:
    IRenderAdapter& renderer_;
};
