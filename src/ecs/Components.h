#pragma once

#include "ecs/Component.h"
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
    std::string texturePath;
    std::string materialName;
    std::string shaderName;
};

struct Hierarchy {
    Entity parent = kInvalidEntity;
    std::vector<Entity> children;
};

struct Spin {
    float speed = 0.0f;
};

template <>
struct IsComponent<Transform> : std::true_type {
};

template <>
struct IsComponent<Tag> : std::true_type {
};

template <>
struct IsComponent<MeshRenderer> : std::true_type {
};

template <>
struct IsComponent<Hierarchy> : std::true_type {
};

template <>
struct IsComponent<Spin> : std::true_type {
};
