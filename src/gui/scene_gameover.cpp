#include "scene_gameover.h"
#include "scene_mainmenu.h"
#include "context.h"

#include <common/highscore_manager.h>

#include <SDL.h>

using namespace hexx::gui;
using namespace hexx::common;

SceneGameOver::SceneGameOver(int ruby_score, int pearl_score)
    : SceneBase(), ruby_score{ruby_score}, pearl_score{pearl_score}
{
    ok_button.set_text(std::move("Return to menu"));
    ok_button.set_rect({100, 200, 200, 24});
    ok_button.set_callback(
        [](Context &ctx)
        {
            ctx.manager.pop();
            auto scene = std::make_unique<SceneMainMenu>();
            ctx.manager.replace(std::move(scene));
        });
}

void SceneGameOver::tick(Context &ctx)
{
}

void SceneGameOver::draw(Context &ctx)
{
    SDL_SetRenderDrawColor(ctx.render, 31, 37, 43, 50);
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

    center_text("Game Over", 100, Color(255, 255, 255));

    center_text("Ruby score: " + std::to_string(ruby_score), 150, COLOR_RUBY);
    center_text("Pearl score: " + std::to_string(pearl_score), 165, COLOR_PEARL);

    if (ruby_score > pearl_score)
    {
        center_text("Ruby wins!", 180, COLOR_RUBY);
    }
    else if (pearl_score > ruby_score)
    {
        center_text("Pearl wins!", 180, COLOR_PEARL);
    }
    else
    {
        center_text("Draw!", 180, Color(255, 255, 0));
    }

    ok_button.draw(ctx);
}

void SceneGameOver::mouse_down(Context &ctx, uint8_t index, int x, int y)
{
    ok_button.mouse_down(ctx, x, y);
}

void SceneGameOver::mouse_up(Context &ctx, uint8_t index, int x, int y)
{
    ok_button.mouse_up(ctx, x, y);
}

void SceneGameOver::mouse_move(Context &ctx, int x, int y)
{
    ok_button.mouse_move(ctx, x, y);
}