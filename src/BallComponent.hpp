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
            transform->position.x = 0.0f;
            velocity.x = std::abs(velocity.x);
        }
        if (transform->position.x >= 960.0f - width)
        {
            transform->position.x = 960.0f - width;
            velocity.x = -std::abs(velocity.x);
        }
        if (transform->position.y <= 0.0f)
        {
            transform->position.y = 0.0f;
            velocity.y = std::abs(velocity.y);
        }
        if (transform->position.y >= 540.0f - height)
        {
            transform->position.y = 540.0f - height;
            velocity.y = -std::abs(velocity.y);
        }
    }

    void OnCollision(GameObject *other) override
    {
        if (!owner || !other) return;

        TransformComponent *my_transform = owner->GetComponent<TransformComponent>();
        TransformComponent *other_transform = other->GetComponent<TransformComponent>();
        ColliderComponent *my_collider = owner->GetComponent<ColliderComponent>();
        ColliderComponent *other_collider = other->GetComponent<ColliderComponent>();

        if (!my_transform || !other_transform || !my_collider || !other_collider) return;


        Vector2 my_size = my_collider->size * my_transform->scale.x;
        Vector2 other_size = other_collider->size * other_transform->scale.x;

        Vector2 my_center = my_transform->position + (my_size * 0.5f);
        Vector2 other_center = other_transform->position + (other_size * 0.5f);

        Vector2 diff = my_center - other_center;

        if(std::abs(diff.x) > std::abs(diff.y))
        {
            velocity.x = (diff.x > 0) ? std::abs(velocity.x) : -std::abs(velocity.x);
        }
        else
        {
            velocity.y = (diff.y > 0) ? std::abs(velocity.y) : -std::abs(velocity.y);
        }

        SDL_Log("Colisión: %s con %s", owner->GetTag().c_str(), other->GetTag().c_str());

    }
};