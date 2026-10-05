#pragma once

#include <cmath>
#include <SDL3/SDL.h>
#include "Component.hpp"
#include "GameObject.hpp"
#include "TransformComponent.hpp"
#include "ColliderComponent.hpp"
#include "Vector2.hpp"

class BallComponent : public Component
{
public:
    Vector2 velocity{220.0f, 180.0f};

    void Update(float dt) override
    {
        if (!owner) return;
        TransformComponent *transform = owner->GetComponent<TransformComponent>();
        if (!transform) return;

        transform->Translate(velocity * dt);
        //Rebote elástico en los bordes de la ventana
        ColliderComponent *collider = owner->GetComponent<ColliderComponent>();
        float width = collider ? collider->size.x * transform->scale.x : 0.0f;
        float height = collider ? collider->size.y * transform->scale.y : 0.0f;

        if (transform->position.x <= 0.0f)
        {
            transform->position.x = 0.0f
            velocity.x = std::abs(velocity.x);
        }//continuar...
    }
}