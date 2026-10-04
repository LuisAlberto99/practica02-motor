#pragma once

#include <SDL3/SDL.h>
#include"Component.hpp"
#include "GameObject.hpp"
#include"TransformComponent.hpp"
#include "Vector2.hpp"

class ColliderComponent : public Component
{
    public:
    Vector2 offset
}