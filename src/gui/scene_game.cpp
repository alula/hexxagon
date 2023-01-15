#include "scene_game.h"
#include "scene_gameover.h"
#include "scene_input.h"
#include "context.h"
#include <common/level_data.h>
#include <common/highscore_manager.h>
#include <common/files.h>

#include <SDL.h>

using namespace hexx::gui;
using namespace hexx::common;
using std::operator""s;

SceneGame::SceneGame() : SceneBase()
{
    board.reset(HexMap<TileState>{LEVEL1_TEMPLATE});
}

void SceneGame::tick(Context &ctx)
{
    if (board.with_computer && board.current_player == Player::Pearl)
    {
        board.clear_highlights();
        const auto move = board.ai_play();

        board.selected_tile = move.from;
        const auto [to_x, to_y] = move.to;

        if (board.try_move(to_x, to_y))
        {
            board.next_player();
        }
    }
    else
    {
        if (clicked_tile != std::pair{-1, -1})
        {
            const auto players_tile_type = board.current_player == Player::Ruby ? TileState::Ruby : TileState::Pearl;
            const auto [to_x, to_y] = clicked_tile;
            clicked_tile = {-1, -1};

            auto tile = board.map.at(to_x, to_y);
            if (*tile == players_tile_type)
            {
                board.clear_highlights();
                board.selected_tile = {to_x, to_y};
                board.highlight_moves(to_x, to_y);
            }
            else if (board.selected_tile != std::pair{-1, -1})
            {
                const auto [from_x, from_y] = board.selected_tile;

                if (board.try_move(to_x, to_y))
                {
                    board.next_player();
                    board.clear_highlights();
                    board.selected_tile = {-1, -1};
                }
            }
        }
    }

    if (board.game_ended())
    {
        HighScoreManager{}.add_score(board.ruby_score, board.pearl_score);

        auto scene = std::make_unique<SceneGameOver>(board.ruby_score, board.pearl_score);
        ctx.manager.push(std::move(scene));
    }
}

void SceneGame::draw(Context &ctx)
{
    const auto clear_color = Color::from_hsl(SDL_GetTicks() / 30.0f, 0.3f, 0.25f, 1.0f);

    SDL_SetRenderDrawColor(ctx.render, clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    SDL_RenderClear(ctx.render);

    const auto offset_x = (400 - board.map.get_width() * 32) / 2;
    const auto offset_y = (300 - board.map.get_height() * 32) / 2;

    for (auto tile = board.map.begin(); tile != board.map.end(); tile++)
    {
        if (*tile == TileState::Void)
            continue;

        const auto pos_x = offset_x + tile.face_x() * 32;
        const auto pos_y = offset_y + tile.face_y() * 32 + ((tile.face_x() & 1) ? 16 : 0);

        ctx.res.hexagon.draw(pos_x, pos_y, BlendMode::Add);

        constexpr auto sprite_offset = (40 - 32) / 2;

        if (*tile == TileState::Ruby)
        {
            ctx.res.ruby.draw(pos_x + sprite_offset, pos_y);
        }
        else if (*tile == TileState::Pearl)
        {
            ctx.res.pearl.draw(pos_x + sprite_offset, pos_y);
        }
    }

    for (const auto &[x, y] : board.highlights)
    {
        const auto pos_x = offset_x + x * 32;
        const auto pos_y = offset_y + y * 32 + ((x & 1) ? 16 : 0);

        ctx.res.highlight.draw(pos_x, pos_y);
    }

    ctx.res.font.builder()
        .with_pos(2, 2)
        .with_text(board.current_player == Player::Ruby ? "This is Ruby's turn!"
                                                        : "This is Pearl's turn!")
        .with_color(board.current_player == Player::Ruby ? COLOR_RUBY : COLOR_PEARL)
        .draw();

    ctx.res.font.builder()
        .with_pos(2, 12)
        .with_text("Ruby: " + std::to_string(board.ruby_score))
        .with_color(COLOR_RUBY)
        .draw();

    ctx.res.font.builder()
        .with_pos(2, 22)
        .with_text("Pearl: " + std::to_string(board.pearl_score))
        .with_color(COLOR_PEARL)
        .draw();

    // res.font.builder()
    //     .with_pos(4, (SDL_GetTicks() / 10) % 300)
    //     .with_color(Color(0, 0, 0))
    //     .with_text("asdf 12345 dupa")
    //     .draw();
}

void SceneGame::mouse_down(Context &ctx, uint8_t index, int x, int y)
{
    const auto offset_x = (400 - board.map.get_width() * 32) / 2;
    const auto offset_y = (300 - board.map.get_height() * 32) / 2;

    for (auto tile = board.map.begin(); tile != board.map.end(); tile++)
    {
        if (*tile == TileState::Void)
            continue;

        const auto pos_x = offset_x + tile.face_x() * 32;
        const auto pos_y = offset_y + tile.face_y() * 32 + ((tile.face_x() & 1) ? 16 : 0);

        Rect mouse_rect{pos_x + 7, pos_y, 26, 32};
        if (mouse_rect.contains(x, y))
        {
            // printf("Clicked tile: %d, %d\n", tile.face_x(), tile.face_y());
            clicked_tile = {tile.face_x(), tile.face_y()};
            break;
        }
    }
}

void SceneGame::mouse_up(Context &ctx, uint8_t index, int x, int y)
{
}

void SceneGame::mouse_move(Context &ctx, int x, int y)
{
}

void SceneGame::key_down(Context &ctx, SDL_Keycode key)
{
    if (key == SDLK_EQUALS)
    {
        auto input_scene = std::make_unique<SceneInput>(
            "Enter the file name of game state save or nothing to cancel:",
            [this](Context &ctx, std::string filename)
            {
                ctx.manager.pop();
                if (filename.empty())
                {
                    return;
                }

                try
                {
                    auto data = board.serialize();
                    write_file(filename, data);
                }
                catch (std::exception &e)
                {
                    ctx.message_box("Error saving game: "s + e.what());
                }
            });
        ctx.manager.push(std::move(input_scene));
    }
}