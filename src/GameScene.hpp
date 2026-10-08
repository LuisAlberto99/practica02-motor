#pragma once

#include <SDL3/SDL.h>
#include "Scene.hpp"
#include "TransformComponent.hpp"
#include "RectRenderComponent.hpp"
#include "PlayerControllerComponent.hpp"
#include "ColliderComponent.hpp"
#include "CollisionManager.hpp"
#include "BallComponent.hpp"

class GameScene : public Scene
{
private:
    CollisionManager m_collisionManager{&m_entities};
    float m_physicsAccumulator{0.0f};
    bool m_debugDraw{false};

public:
    explicit GameScene(SceneManager *manager)
        : Scene(manager, "GameScene") {}

    void Init() override
    {
        m_entities.clear();

        // --- Jugador móvil (Verde) ---
        auto player = std::make_unique<GameObject>("Player");
        player->AddComponent<TransformComponent>(Vector2{440.0f, 240.0f}, Vector2{1.0f, 1.0f});
        player->AddComponent<RectRenderComponent>(Vector2{60.0f, 60.0f}, SDL_Color{60, 180, 100, 255});
        player->AddComponent<PlayerControllerComponent>(300.0f, true);
        player->AddComponent<ColliderComponent>(Vector2{60.0f, 60.0f});
        m_entities.push_back(std::move(player));

        // --- Obstáculo estático (Rojo) ---
        auto obstacle = std::make_unique<GameObject>("Obstacle");
        obstacle->AddComponent<TransformComponent>(Vector2{180.0f, 140.0f}, Vector2{1.0f, 1.0f});
        obstacle->AddComponent<RectRenderComponent>(Vector2{80.0f, 80.0f}, SDL_Color{220, 70, 70, 255});
        obstacle->AddComponent<ColliderComponent>(Vector2{80.0f, 80.0f});
        m_entities.push_back(std::move(obstacle));

        // --- Pelota móvil autónoma (Amarilla) ---
        auto ball = std::make_unique<GameObject>("Ball");
        ball->AddComponent<TransformComponent>(Vector2{468.0f, 80.0f}, Vector2{1.0f, 1.0f});
        ball->AddComponent<RectRenderComponent>(Vector2{24.0f, 24.0f}, SDL_Color{240, 210, 60, 255});
        ball->AddComponent<ColliderComponent>(Vector2{24.0f, 24.0f});
        ball->AddComponent<BallComponent>();
        m_entities.push_back(std::move(ball));

        m_physicsAccumulator = 0.0f;
    }

    void HandleEvent(const SDL_Event &event) override;

    void Update(float dt) override
    {
        constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;
        m_physicsAccumulator += dt;

        while (m_physicsAccumulator >= FIXED_TIMESTEP)
        {
            for (auto &entity : m_entities)
            {
                entity->Update(FIXED_TIMESTEP);
            }
            m_collisionManager.CheckCollisions();
            m_physicsAccumulator -= FIXED_TIMESTEP;
        }
    }

    void Render(SDL_Renderer *renderer) override
    {
        SDL_SetRenderDrawColor(renderer, 25, 25, 30, 255);
        SDL_RenderClear(renderer);

        for (auto &entity : m_entities)
        {
            entity->Render(renderer);
        }

        if (m_debugDraw)
        {
            for (auto &entity : m_entities)
            {
                if (auto *col = entity->GetComponent<ColliderComponent>())
                {
                    col->RenderDebug(renderer);
                }
            }
        }
    }

    void ToggleDebugDraw()
    {
        m_debugDraw = !m_debugDraw;
        SDL_Log("Debug Draw: %s", m_debugDraw ? "ACTIVADO" : "DESACTIVADO");
    }
};