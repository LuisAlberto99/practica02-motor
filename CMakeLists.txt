cmake_minimum_required(VERSION 3.16)
project(practica05_motor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscamos SDL3
find_package(SDL3 QUIET)
if(NOT SDL3_FOUND)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL3 REQUIRED IMPORTED_TARGET sdl3)
    set(SDL3_TARGET PkgConfig::SDL3)
else()
    set(SDL3_TARGET SDL3::SDL3)
endif()


# Declaración de ejecutables y fuentes
add_executable(${PROJECT_NAME}
src/main.cpp
src/Vector2.hpp
src/Component.hpp
src/GameObject.hpp
src/TransformComponent.hpp
src/RectRenderComponent.hpp
src/PlayerControllerComponent.hpp
src/ColliderComponent.hpp
src/CollisionManager.hpp
src/BallComponent.hpp
src/Scene.hpp
src/SceneManager.hpp
src/TitleScene.hpp
src/GameScene.hpp
src/PauseScene.hpp
src/GameOverScene.hpp

)

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL3_TARGET})


