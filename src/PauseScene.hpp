
#pragma once

#include <SDL3/SDL.h>
#include "Scene.hpp"

class PauseScene : public Scene
{
    public:
        explicit PauseScene(SceneManager *manager)
            : Scene(manager, "PauseScene") {}
        
        void HandleEvent(const SDL_Event &event) override
        {
            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if(event.key.scancode == SDL_SCANCODE_P || event.key.scancode == SDL_SCANCODE_ESCAPE)
                {
                    m_manager->PopScene();
                }
            }
        }

        void Render(SDL_Renderer *renderer) override
        {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 175);
            SDL_FRect screen_overlay{0.0f, 0.0f, 960.0f, 540.0f};
            SDL_RenderFillRect(renderer, &screen_overlay);
            
            SDL_SetRenderDrawColor(renderer, 230, 200, 60, 225);
            SDL_FRect panel{340.0f, 200.0f, 280.0f, 140.0f};
            SDL_RenderFillRect(renderer, &panel);

            SDL_SetRenderDrawColor(renderer, 30, 30, 35, 255);
            SDL_FRect bar1{440.0f, 235.0f, 25.0f, 70.0f};
            SDL_FRect bar2{495.0f, 235.0f, 25.0f, 70.0f};
            SDL_RenderFillRect(renderer, &bar1);
            SDL_RenderFillRect(renderer, &bar2);
           
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
            
        }

};