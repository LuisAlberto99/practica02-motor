#define SDL_MAIN_USE_CALLBACKS 1

#include <vector>
#include <memory>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Vector2.hpp"
#include "GameObject.hpp"
#include "TransformComponent.hpp"
#include "RectRenderComponent.hpp"
#include "PlayerControllerComponent.hpp"

void SDL_LogPlatformInfo();

struct AppState
{
    SDL_Renderer *renderer{nullptr};
    SDL_Window *window{nullptr};

    // Temporizador para Delta Time
    Uint64 last_ticks{0};
    float physics_accumulator{0.0f};

    // Colección de todas las entidades activas en el mundo
    std::vector<std::unique_ptr<GameObject>> entities;
} appstate;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error al inicializar SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, "60");
    SDL_LogPlatformInfo();

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    if (!SDL_CreateWindowAndRenderer("Práctica 03 - GameObject y Componentes", 960, 540, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error al crear ventana o renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Log("Renderer Driver activo: %s", SDL_GetRendererName(renderer));

    ::appstate.window = window;
    ::appstate.renderer = renderer;
    ::appstate.last_ticks = SDL_GetTicks();

    // --- Ensamblamos al Jugador por composición ---
    auto player = std::make_unique<GameObject>("Player");
    player->AddComponent<TransformComponent>(Vector2{440.0f, 240.0f}, Vector2{1.0f, 1.0f});
    player->AddComponent<RectRenderComponent>(Vector2{60.0f, 60.0f}, SDL_Color{60, 180, 100, 255});
    player->AddComponent<PlayerControllerComponent>(300.0f, true);
    ::appstate.entities.push_back(std::move(player));

    // --- Entidad Obstáculo: reutiliza Transform y RectRender, sin PlayerController ---
    auto obstacle = std::make_unique<GameObject>("Obstacle");
    obstacle->AddComponent<TransformComponent>(Vector2{150.0f, 120.0f}, Vector2{1.5f, 1.5f});
    obstacle->AddComponent<RectRenderComponent>(Vector2{40.0f, 40.0f}, SDL_Color{220, 70, 70, 255});
    ::appstate.entities.push_back(std::move(obstacle));

    *appstate = &::appstate;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *app = static_cast<AppState *>(appstate);

    // 1. Medición de Delta Time en segundos
    Uint64 current_ticks = SDL_GetTicks();
    float delta_time = static_cast<float>(current_ticks - app->last_ticks) / 1000.0f;
    app->last_ticks = current_ticks;

    if (delta_time > 0.05f)
    {
        delta_time = 0.05f;
    }

    // 2. Fase de Actualización (timestep fijo)
    constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;
    app->physics_accumulator += delta_time;

    while (app->physics_accumulator >= FIXED_TIMESTEP)
    {
        for (auto &entity : app->entities)
        {
            entity->Update(FIXED_TIMESTEP);
        }
        app->physics_accumulator -= FIXED_TIMESTEP;
    }

    // 3. Fase de Renderizado
    SDL_SetRenderDrawColor(app->renderer, 25, 25, 30, 255);
    SDL_RenderClear(app->renderer);

    for (auto &entity : app->entities)
    {
        entity->Render(app->renderer);
    }

    SDL_RenderPresent(app->renderer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    AppState *app = static_cast<AppState *>(appstate);
    if (app)
    {
        app->entities.clear();
        SDL_DestroyRenderer(app->renderer);
        SDL_DestroyWindow(app->window);
    }
    SDL_Quit();
}

void SDL_LogPlatformInfo()
{
    SDL_Log("Plataforma: %s", SDL_GetPlatform());
    SDL_Log("Cores lógicos de CPU: %d", SDL_GetNumLogicalCPUCores());
    SDL_Log("RAM total: %d MB", SDL_GetSystemRAM());
    SDL_Log("Driver de video: %s", SDL_GetCurrentVideoDriver());
}