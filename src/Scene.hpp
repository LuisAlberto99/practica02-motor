#pragma once

#include <vector>
#include <memory>
#include <string>
#include <SDL3/SDL.h>
#include "GameObject.hpp"

class Scene
{
protected:
    SceneManager *m_manager{nullptr};
    std::vector<std::unique_ptr<GameObject>> m_entities;
    std::string m_name;

public:
    Scene(SceneManager *manager, std::strings name)
        : m_manager(manager), m_name(std::move(name)) {}

    virtual ~Scene() = default;
}