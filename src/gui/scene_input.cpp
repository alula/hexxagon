#include "scene_input.h"

#include <SDL.h>

using namespace hexx::gui;

SceneInput::SceneInput(std::string message, std::function<void(Context &, std::string)> callback) : SceneBase(), message{message}, callback{callback}, pos{}, input{}
{
    ok_button.set_text("OK");
    ok_button.set_rect({100, 200, 200, 24});
    ok_button.set_callback(
        [this](Context &ctx)
        {
            this->callback(ctx, this->input);
        });
}

void SceneInput::tick(Context &ctx)
{
}

void SceneInput::draw(Context &ctx)
{
    SDL_SetRenderDrawColor(ctx.render, 31, 37, 43, 255);
    SDL_RenderClear(ctx.render);

    auto center_text = [&](std::string const &text, int y, Color const &color)
    {
        const auto [text_width, text_height] = ctx.res.font.get_text_size(text);
        const auto text_x = (400 - text_width) / 2;
        ctx.res.font.builder()
            .with_pos(text_x, y)
            .with_color(color)
            .with_text(text)
            .draw();
    };

    center_text(message, 50, Color(255, 255, 255));

    ctx.res.font.builder()
        .with_pos(40, 100)
        .with_color(Color(255, 255, 255))
        .with_text(input)
        .draw();

    if (SDL_GetTicks() / 500 % 2 == 0)
    {
        fill_rect(ctx.render, {40 + ctx.res.font.get_text_size(input.substr(0, pos)).first, 100, 2, 10}, Color(255, 255, 255));
    }

    ok_button.draw(ctx);
}

void SceneInput::mouse_down(Context &ctx, uint8_t index, int x, int y)
{
    ok_button.mouse_down(ctx, x, y);
}

void SceneInput::mouse_up(Context &ctx, uint8_t index, int x, int y)
{
    ok_button.mouse_up(ctx, x, y);
}

void SceneInput::mouse_move(Context &ctx, int x, int y)
{
    ok_button.mouse_move(ctx, x, y);
}

void SceneInput::key_down(Context &ctx, int key)
{
    switch (key)
    {
    case SDLK_BACKSPACE:
        if (pos > 0)
        {
            input.erase(pos - 1, 1);
            pos--;
        }
        break;
    case SDLK_DELETE:
        if (pos < input.size())
        {
            input.erase(pos, 1);
        }
        break;
    case SDLK_LEFT:
        if (pos > 0)
        {
            pos--;
        }
        break;
    case SDLK_RIGHT:
        if (pos < input.size())
        {
            pos++;
        }
        break;
    }
}

void SceneInput::text_input(Context &ctx, std::string const &text)
{
    input.insert(pos, text);
    pos += text.size();
}