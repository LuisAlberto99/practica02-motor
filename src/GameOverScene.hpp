#pragma once

#include <SDL3/SDL.h>
#include "Scene.hpp"

class GameOverScene : public Scene
{
public:
    explicit GameOverScene(SceneManager *manager)
        : Scene(manager, "GameOverScene") {}

    void HandleEvent(const SDL_Event &event) override;

    void Render(SDL_Renderer *renderer) override
    {
        // Fondo rojizo de fin de partida
        SDL_SetRenderDrawColor(renderer, 45, 15, 20, 255);
        SDL_RenderClear(renderer);

        // Cartel central de "Fin de Partida"
        SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
        SDL_FRect title_rect{280.0f, 150.0f, 400.0f, 110.0f};
        SDL_RenderFillRect(renderer, &title_rect);

        // Botón: Reiniciar partida (amarillo)
        SDL_SetRenderDrawColor(renderer, 230, 200, 60, 255);
        SDL_FRect restart_rect{300.0f, 320.0f, 170.0f, 50.0f};
        SDL_RenderFillRect(renderer, &restart_rect);

        // Botón: Menú principal (azul)
        SDL_SetRenderDrawColor(renderer, 70, 130, 220, 255);
        SDL_FRect menu_rect{490.0f, 320.0f, 170.0f, 50.0f};
        SDL_RenderFillRect(renderer, &menu_rect);
    }
};