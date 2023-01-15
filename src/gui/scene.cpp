#include "scene.h"
#include "context.h"

#include <SDL.h>

using namespace hexx::gui;

void SceneManager::tick(Context &ctx)
{
    if (!scene_stack.empty())
    {
        scene_stack.back()->tick(ctx);
    }
}

void SceneManager::draw(Context &ctx)
{
    for (auto &scene : scene_stack)
    {
        SDL_SetRenderTarget(renderer, scene->back_buffer->get_inner());

        scene->draw(ctx);

        SDL_SetRenderTarget(renderer, nullptr);
    }

    for (auto &scene : scene_stack)
    {
        SDL_RenderCopy(renderer, scene->back_buffer->get_inner(), nullptr, nullptr);
    }
}

void SceneManager::key_down(Context &ctx, int key)
{
    if (!scene_stack.empty())
    {
        scene_stack.back()->key_down(ctx, key);
    }
}

void SceneManager::key_up(Context &ctx, int key)
{
    if (!scene_stack.empty())
    {
        scene_stack.back()->key_up(ctx, key);
    }
}

void SceneManager::mouse_down(Context &ctx, uint8_t index, int x, int y)
{
    if (!scene_stack.empty())
    {
        scene_stack.back()->mouse_down(ctx, index, x, y);
    }
}

void SceneManager::mouse_up(Context &ctx, uint8_t index, int x, int y)
{
    if (!scene_stack.empty())
    {
        scene_stack.back()->mouse_up(ctx, index, x, y);
    }
}

void SceneManager::mouse_move(Context &ctx, int x, int y)
{
    if (!scene_stack.empty())
    {
        scene_stack.back()->mouse_move(ctx, x, y);
    }
}

void SceneManager::text_input(Context &ctx, std::string const &text)
{
    if (!scene_stack.empty())
    {
        scene_stack.back()->text_input(ctx, text);
    }
}

void SceneManager::push(std::unique_ptr<SceneBase> &&scene)
{
    scene->back_buffer = std::make_shared<Texture>();
    scene->back_buffer->initialize_render_target(renderer, DISPLAY_WIDTH, DISPLAY_HEIGHT);

    scene_stack.push_back(std::move(scene));
}

void SceneManager::pop()
{
    scene_stack.pop_back();
}

void SceneManager::replace(std::unique_ptr<SceneBase> &&scene)
{
    pop();
    push(std::move(scene));
}