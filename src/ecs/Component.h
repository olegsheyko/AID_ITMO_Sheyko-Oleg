#pragma once

#include <type_traits>

struct Component {
};

template <typename T>
struct IsComponent : std::is_base_of<Component, T> {
};

template <typename T>
inline constexpr bool kIsComponent = IsComponent<T>::value;
