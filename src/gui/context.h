#pragma once

#include "render.h"
#include "scene.h"
#include "resources.h"

namespace hexx::gui
{
    struct Context
    {
        SDL_Renderer *render;

        ResourceInstances res;
        SceneManager manager;
        bool debug;
        bool running;

        Context(SDL_Renderer *render)
            : render{render}, res{render}, manager{render}, debug{false}, running{true}
        {
        }

        void message_box(std::string const &message);
    };
};