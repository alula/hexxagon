project(hexxagon)

cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDL_STATIC_ENABLED_BY_DEFAULT ON)
set(SDL_SHARED_ENABLED_BY_DEFAULT OFF)
set(SDL_AUDIO OFF)

include(FetchContent)
FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.26.0
)
FetchContent_MakeAvailable(SDL2)

add_library(hexxagon_common 
    src/common/board.cpp
    src/common/files.cpp
    src/common/highscore_manager.cpp
    src/common/sequencer.cpp
)

target_include_directories(hexxagon_common PUBLIC 
    src/common
)

add_executable(hexxagon_cli
    src/cli/main.cpp
    src/cli/utils.cpp
)

add_executable(hexxagon_gui WIN32
    src/gui/main.cpp
    src/gui/render.cpp
    src/gui/resources.cpp
    src/gui/button.cpp
    src/gui/scene.cpp
    src/gui/scene_game.cpp
    src/gui/scene_gameover.cpp
    src/gui/scene_input.cpp
    src/gui/scene_mainmenu.cpp
)

target_include_directories(hexxagon_cli PUBLIC 
    src
    src/cli
)

target_include_directories(hexxagon_gui PUBLIC 
    src
    src/gui
    ${SDL2_SOURCE_DIR}/include
)

target_link_libraries(hexxagon_cli hexxagon_common)
target_link_libraries(hexxagon_gui hexxagon_common SDL2-static)