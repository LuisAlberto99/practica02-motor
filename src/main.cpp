#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Vector2.hpp"

void SDL_LogPlatformInfo();
struct Character
{
    Vector2 position{440.0f, 240.0f};
    Vector2 size{60.0f, 60.0f};
    float speed{300.0f};
    SDL_Color color{60, 180, 100, 255};
};
struct AppState
{
    SDL_Renderer *renderer{nullptr};
    SDL_Window *window{nullptr};

    // Temporizador para Delta Time
    Uint64 last_ticks{0};

    // Posición del jugador usando Vector2
    //Vector2 player_pos{440.0f, 240.0f};
    //float rect_width{60.0f};
    //float rect_height{60.0f};
    //float speed{300.0f}; // Píxeles por segundo
    Character player;

    float physics_accumulator{0.0f};
} appstate;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error al inicializar SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Sugerir a la plataforma una tasa objetivo de 60 FPS
    SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, "60");

    SDL_LogPlatformInfo();

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    if (!SDL_CreateWindowAndRenderer("Práctica 02 - Vectores y Movimiento", 960, 540, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error al crear ventana o renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Log("Renderer Driver activo: %s", SDL_GetRendererName(renderer));

    ::appstate.window = window;
    ::appstate.renderer = renderer;
    ::appstate.last_ticks = SDL_GetTicks();

    *appstate = &::appstate;
    return SDL_APP_CONTINUE;
}

//void PhysicsUpdate(Character &character, const Vector2 &direction, float fixed_dt)
//{
//    Vector2 displacement = direction * (character.speed * fixed_dt);
//    character.position = character.position + displacement;
//}
void PhysicsUpdate(Character &character, const Vector2 &direction, float fixed_dt)
{
    Vector2 displacement = direction * (character.speed * fixed_dt);
    character.position = character.position + displacement;

    // Clamping: confinar el personaje dentro de la ventana (960x540)
    if (character.position.x < 0.0f)
    {
        character.position.x = 0.0f;
    }
    if (character.position.x > 960.0f - character.size.x)
    {
        character.position.x = 960.0f - character.size.x;
    }
    if (character.position.y < 0.0f)
    {
        character.position.y = 0.0f;
    }
    if (character.position.y > 540.0f - character.size.y)
    {
        character.position.y = 540.0f - character.size.y;
    }
}
SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *app = static_cast<AppState *>(appstate);

    // 1. Medición de Delta Time en segundos
    Uint64 current_ticks = SDL_GetTicks();
    float delta_time = static_cast<float>(current_ticks - app->last_ticks) / 1000.0f;
    app->last_ticks = current_ticks;

    // Evitar saltos de tiempo excesivos si el sistema operativo pausa el proceso
    if (delta_time > 0.05f)
    {
        delta_time = 0.05f;
    }

    // 2. Fase de Actualización (Update)
    const bool *keys = SDL_GetKeyboardState(nullptr);

    Vector2 input_dir{0.0f, 0.0f};
    if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])    input_dir.y -= 1.0f;
    if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])  input_dir.y += 1.0f;
    if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])  input_dir.x -= 1.0f;
    if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) input_dir.x += 1.0f;

    // Normalización: si el jugador se está moviendo, normalizamos el vector
    if (input_dir.length_squared() > 0.0f)
    {
        input_dir = input_dir.normalized();
    }

    // Calculamos el desplazamiento: velocidad escalar * dirección unitaria * delta_time
    //Vector2 displacement = input_dir * (app->speed * delta_time);
    //app->player_pos = app->player_pos + displacement;
    //Vector2 displacement = input_dir * (app->player.speed * delta_time);
    //app->player.position = app->player.position + displacement;
    constexpr float FIXED_TIMESTEP = 1.0f / 60.0f; // 60 Hz estables
app->physics_accumulator += delta_time;

while (app->physics_accumulator >= FIXED_TIMESTEP)
{
    PhysicsUpdate(app->player, input_dir, FIXED_TIMESTEP);
    app->physics_accumulator -= FIXED_TIMESTEP;
}
    // 3. Fase de Renderizado
    SDL_SetRenderDrawColor(app->renderer, 30, 30, 35, 255);
    SDL_RenderClear(app->renderer);

    //SDL_SetRenderDrawColor(app->renderer, 60, 180, 100, 255);
    //SDL_FRect player_rect{app->player.position.x, app->player.position.y, app->player.size.x, app->player.size.y};
    SDL_SetRenderDrawColor(app->renderer, app->player.color.r, app->player.color.g, app->player.color.b, app->player.color.a);
SDL_FRect player_rect{app->player.position.x, app->player.position.y, app->player.size.x, app->player.size.y};
    SDL_RenderFillRect(app->renderer, &player_rect);

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