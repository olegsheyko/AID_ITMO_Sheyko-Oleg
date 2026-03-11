#pragma once

#include "math/MathTypes.h"
#include "ecs/Entity.h"
#include "render/RenderTypes.h"

#include <string>
#include <vector>

struct Transform {
    Vec3 position{};
    float rotation = 0.0f;
    Vec3 scale{1.0f, 1.0f, 1.0f};
};

struct Tag {
    std::string name;
};

struct MeshRenderer {
    PrimitiveType primitive = PrimitiveType::Triangle;
    Vec4 color{};
};

struct Hierarchy {
    Entity parent = kInvalidEntity;
    std::vector<Entity> children;
};

struct Spin {
    float speed = 0.0f;
};
