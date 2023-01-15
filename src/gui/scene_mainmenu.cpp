#include "scene_mainmenu.h"
#include "scene_game.h"
#include "scene_input.h"
#include "context.h"

#include <common/highscore_manager.h>
#include <common/files.h>

#include <SDL.h>

using namespace hexx::gui;
using namespace hexx::common;

using std::operator""s;

SceneMainMenu::SceneMainMenu() : SceneBase()
{
    single_player_button.set_text(std::move("Player vs Computer"));
    single_player_button.set_rect({100, 60, 200, 24});
    single_player_button.set_callback(
        [](Context &ctx)
        {
            auto scene = std::make_unique<SceneGame>();
            scene->board.with_computer = true;
            ctx.manager.replace(std::move(scene));
        });

    multi_player_button.set_text(std::move("Player vs Player"));
    multi_player_button.set_rect({100, 90, 200, 24});
    multi_player_button.set_callback(
        [](Context &ctx)
        {
            auto scene = std::make_unique<SceneGame>();
            ctx.manager.replace(std::move(scene));
        });

    load_button.set_text(std::move("Load from file"));
    load_button.set_rect({100, 120, 200, 24});
    load_button.set_callback(
        [](Context &ctx)
        {
            auto input_scene = std::make_unique<SceneInput>(
                "Enter filename or nothing to cancel:",
                [](Context &ctx, std::string filename)
                {
                    ctx.manager.pop();
                    if (filename.empty())
                    {
                        return;
                    }

                    try
                    {
                        auto scene = std::make_unique<SceneGame>();
                        auto data = read_file(filename);
                        scene->board.deserialize(data);
                        ctx.manager.replace(std::move(scene));
                    }
                    catch (std::exception &e)
                    {
                        ctx.message_box("Error loading game: "s + e.what());
                    }
                });
            ctx.manager.push(std::move(input_scene));
        });

    quit_button.set_text(std::move("Quit"));
    quit_button.set_rect({100, 150, 200, 24});
    quit_button.set_callback(
        [](Context &ctx)
        {
            ctx.running = false;
        });
}

void SceneMainMenu::tick(Context &ctx)
{
}

void SceneMainMenu::draw(Context &ctx)
{
    using std::operator""s;

    SDL_SetRenderDrawColor(ctx.render, 31, 37, 43, 255);
    SDL_RenderClear(ctx.render);

    const auto center = (DISPLAY_WIDTH - (ctx.res.logo_hexx.get_rect().w * 2 + ctx.res.logo_agon.get_rect().w * 2)) / 2;
    const auto logo_pos_y = 30;

    ctx.res.logo_hexx.draw_scaled(center, logo_pos_y,
                                  ctx.res.logo_hexx.get_rect().w * 2, ctx.res.logo_hexx.get_rect().h * 2);
    ctx.res.logo_agon.draw_scaled(center + ctx.res.logo_hexx.get_rect().w * 2, logo_pos_y,
                                  ctx.res.logo_agon.get_rect().w * 2, ctx.res.logo_agon.get_rect().h * 2);

    single_player_button.draw(ctx);
    multi_player_button.draw(ctx);
    load_button.draw(ctx);
    quit_button.draw(ctx);

    ctx.res.font.builder()
        .with_text("Press '=' in game to save the game state to file")
        .with_color(Color(255, 255, 255))
        .with_pos(10, DISPLAY_HEIGHT - 20)
        .draw();

    HighScoreManager high_score_manager{};
    if (!high_score_manager.scores.empty())
    {
        // printf("\nHigh scores:\n");
        ctx.res.font.builder()
            .with_text("High scores")
            .with_color(Color(255, 255, 0))
            .with_pos(10, DISPLAY_HEIGHT - 100)
            .draw();
        for (auto i = 0; auto const &score : high_score_manager.scores)
        {
            auto outcome = score.winner == HighScoreManager::Winner::Ruby    ? "Ruby won"
                           : score.winner == HighScoreManager::Winner::Pearl ? "Pearl won"
                                                                             : "Draw";

            ctx.res.font.builder()
                .with_text("Ruby "s + std::to_string(score.ruby) + " - " + std::to_string(score.pearl) + " Pearl, " + outcome)
                .with_color(Color(200, 200, 200))
                .with_pos(10, DISPLAY_HEIGHT - 90 + i++ * 10)
                .draw();
        }
    }
}

void SceneMainMenu::mouse_down(Context &ctx, uint8_t index, int x, int y)
{
    single_player_button.mouse_down(ctx, x, y);
    multi_player_button.mouse_down(ctx, x, y);
    load_button.mouse_down(ctx, x, y);
    quit_button.mouse_down(ctx, x, y);
}

void SceneMainMenu::mouse_up(Context &ctx, uint8_t index, int x, int y)
{
    single_player_button.mouse_up(ctx, x, y);
    multi_player_button.mouse_up(ctx, x, y);
    load_button.mouse_up(ctx, x, y);
    quit_button.mouse_up(ctx, x, y);
}

void SceneMainMenu::mouse_move(Context &ctx, int x, int y)
{
    single_player_button.mouse_move(ctx, x, y);
    multi_player_button.mouse_move(ctx, x, y);
    load_button.mouse_move(ctx, x, y);
    quit_button.mouse_move(ctx, x, y);
}