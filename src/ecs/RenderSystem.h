#pragma once

#include "ecs/System.h"

class IRenderAdapter;
class World;

class RenderSystem : public RenderSystemBase {
public:
    explicit RenderSystem(IRenderAdapter& renderer);

    void render(World& world) override;

private:
    IRenderAdapter& renderer_;
};
