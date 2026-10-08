#pragma once

#include <SDL3/SDL.h>
#include <Scene.hpp>

class TitleScene : public Scene
{
public:
    explicit TitleScene(SceneManager *manager)
        : Scene(manager, "TitleScene") {}

    void HandleEvent(const SDL_Event &event) override;

    void Render(SDL_Renderer *renderer) override
    {
        SDL_SetRenderDrawColor(renderer, 20, 25, 45, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
        SDL_FRect title_rect{280.0f, 180.0f, 400.0f, 110.0f};
        SDL_RenderFillRect(renderer, &title_rect);

        SDL_SetRenderDrawColor(renderer, 50, 200, 120, 255);
        SDL_FRect start_rect{380.0f, 360.0f, 200.0f, 40.0f};
        SDL_RenderFillRect(renderer, &start_rect);

    }
};