#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "SceneManager.hpp"
#include "TitleScene.hpp"
#include "GameScene.hpp"
#include "PauseScene.hpp"
#include "GameOverScene.hpp"

void SDL_LogPlatformInfo();

// --- Definiciones diferidas de HandleEvent (dependencias circulares resueltas aquí) ---

void TitleScene::HandleEvent(const SDL_Event &event)
{
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        if (event.key.scancode == SDL_SCANCODE_SPACE || event.key.scancode == SDL_SCANCODE_RETURN)
        {
            m_manager->ChangeScene(std::make_unique<GameScene>(m_manager));
        }
    }
}

void GameScene::HandleEvent(const SDL_Event &event)
{
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        if (event.key.scancode == SDL_SCANCODE_F1)
        {
            ToggleDebugDraw();
        }
        else if (event.key.scancode == SDL_SCANCODE_P || event.key.scancode == SDL_SCANCODE_ESCAPE)
        {
            m_manager->PushScene(std::make_unique<PauseScene>(m_manager));
        }
        else if (event.key.scancode == SDL_SCANCODE_G)
        {
            m_manager->ChangeScene(std::make_unique<GameOverScene>(m_manager));
        }
    }
}

void GameOverScene::HandleEvent(const SDL_Event &event)
{
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        if (event.key.scancode == SDL_SCANCODE_R)
        {
            m_manager->ChangeScene(std::make_unique<GameScene>(m_manager));
        }
        else if (event.key.scancode == SDL_SCANCODE_M || event.key.scancode == SDL_SCANCODE_ESCAPE)
        {
            m_manager->ChangeScene(std::make_unique<TitleScene>(m_manager));
        }
    }
}

// --- Aplicación ---

struct AppState
{
    SDL_Renderer *renderer{nullptr};
    SDL_Window *window{nullptr};

    Uint64 last_ticks{0};

    // El SceneManager ahora gobierna el estado completo de la aplicación
    SceneManager sceneManager;
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

    if (!SDL_CreateWindowAndRenderer("Práctica 05 - Manejo de Escenas", 960, 540, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error al crear ventana o renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Log("Renderer Driver activo: %s", SDL_GetRendererName(renderer));

    ::appstate.window = window;
    ::appstate.renderer = renderer;
    ::appstate.last_ticks = SDL_GetTicks();

    // Cargamos la pantalla de título como estado inicial
    ::appstate.sceneManager.ChangeScene(std::make_unique<TitleScene>(&::appstate.sceneManager));
    ::appstate.sceneManager.ProcessPendingChanges();

    *appstate = &::appstate;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *app = static_cast<AppState *>(appstate);

    // 1. Medición de Delta Time
    Uint64 current_ticks = SDL_GetTicks();
    float delta_time = static_cast<float>(current_ticks - app->last_ticks) / 1000.0f;
    app->last_ticks = current_ticks;

    if (delta_time > 0.05f)
    {
        delta_time = 0.05f;
    }

    // 2. Procesar transiciones diferidas (punto neutro del ciclo)
    app->sceneManager.ProcessPendingChanges();

    // 3. Actualizar solo la escena en la cima de la pila
    app->sceneManager.Update(delta_time);

    // 4. Renderizar toda la pila, de fondo a cima
    app->sceneManager.Render(app->renderer);
    SDL_RenderPresent(app->renderer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    AppState *app = static_cast<AppState *>(appstate);

    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }

    if (app && app->sceneManager.HasScenes())
    {
        app->sceneManager.HandleEvent(*event);
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    AppState *app = static_cast<AppState *>(appstate);
    if (app)
    {
        app->sceneManager.Clear();
        app->sceneManager.ProcessPendingChanges();
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