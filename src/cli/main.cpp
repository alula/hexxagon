#include <cstdio>
#include <iostream>
#include <array>
#include <ranges>
#include <numeric>
#include <algorithm>

#include <common/board.h>
#include <common/level_data.h>
#include <common/highscore_manager.h>
#include <common/files.h>

#include "utils.h"
#include "ansi.h"

using std::operator""s;

using namespace hexx::common;
using namespace hexx::cli;

bool Ansi::enable_color = true;

/**
 * @brief Save the current board state to a file.
 *
 * @param board instance of Board to save
 */
void save_board(Board const &board)
{
    printf("Enter the file name of game state save or nothing to cancel: ");

    std::string filename;
    std::cin >> filename;

    if (filename == "")
    {
        return;
    }

    auto data = board.serialize();
    write_file(filename, data);

    printf("Board saved to '%s'.\n", filename.c_str());
}

/**
 * @brief Print the board state to the console.
 *
 * @param board instance of Board to print
 * @param highlight_col selected column to highlight
 * @param highlight_rows selected rows to highlight
 */
void print_board(Board const &board, int highlight_col = -1, std::vector<int> const &highlight_rows = {})
{
    std::vector<std::string> lines;

    lines.resize(board.map.get_height() * 2, repeat_str("   ", board.map.get_width()));

    for (auto tile = board.map.cbegin(); tile != board.map.cend(); tile++)
    {
        const auto line = tile.face_y() * 2 + (tile.face_x() % 2);
        const auto col = tile.face_x() * 3 + 1;

        char c = ' ';
        switch (*tile)
        {
        case TileState::Empty:
            c = '.';
            if (std::ranges::find(board.highlights, std::pair{tile.face_x(), tile.face_y()}) != board.highlights.end())
            {
                c = 'x';
            }
            break;
        case TileState::Ruby:
            c = '#';
            break;
        case TileState::Pearl:
            c = 'O';
            break;
        default:
            break;
        }

        lines[line][col] = c;
    }

    if (highlight_col > -1)
    {
        char letter = 'a';
        for (auto const &y : highlight_rows)
        {
            auto tile = board.map.at(highlight_col, y);

            const auto line = tile.face_y() * 2 + (tile.face_x() % 2);
            const auto col = tile.face_x() * 3;

            lines[line][col] = letter++;
        }
    }

    for (auto &line : lines)
    {
        replace_string(line, "#", Ansi{}.bold().red().buf + "#" + Ansi{}.reset().buf);
        replace_string(line, "O", Ansi{}.bold().white().buf + "O" + Ansi{}.reset().buf);
        replace_string(line, ".", Ansi{}.blue().buf + "." + Ansi{}.reset().buf);
        replace_string(line, "x", Ansi{}.bold().blue().buf + "x" + Ansi{}.reset().buf);
    }

    lines[0].append(Ansi{}.bold().red().buf + " # Ruby:  "s + std::to_string(board.ruby_score) + Ansi{}.reset().buf);
    lines[1].append(Ansi{}.bold().white().buf + " O Pearl: "s + std::to_string(board.pearl_score) + Ansi{}.reset().buf);

    for (auto const &line : lines)
    {
        printf("%s\n", line.c_str());
    }

    if (highlight_col == -1)
    {
        printf("%s a  b  c  d  e  f  g  h  i%s\n\n", Ansi{}.bold().green().buf.c_str(), Ansi{}.reset().buf.c_str());
    }
}

/**
 * @brief Prompt the user to select a tile from the board.
 *
 * @param board instance of Board to prompt from
 * @return std::pair<int, int> coordinates of the selected tile
 */
std::pair<int, int> prompt_tile_from(Board const &board)
{
    int x{};
    int y{};

    for (;;)
    {
        print_board(board);
        auto letters = letter_vec(board.map.get_width());
        auto letter_range = first_last(letters);
        letters.push_back('=');
        auto letter = prompt_option_loop(letters, "Enter letter corresponding to the column "s + letter_range);

        if (letter == '=')
        {
            save_board(board);
            continue;
        }

        x = letter - 'a';

        std::vector<int> pickable_y_tiles{};
        auto target_tile = board.current_player == Player::Ruby ? TileState::Ruby : TileState::Pearl;

        for (int i = 0; i < board.map.get_height(); i++)
        {
            auto tile = board.map.at(x, i);

            if (*tile == target_tile)
            {
                if (!board.get_possible_moves(x, i).empty())
                    pickable_y_tiles.push_back(i);
            }
        }

        if (pickable_y_tiles.empty())
        {
            printf("No pickable tiles in that column!\n");
            continue;
        }

        print_board(board, x, pickable_y_tiles);
        letters = letter_vec(pickable_y_tiles.size());
        letter_range = first_last(letters);
        letters.push_back('/');
        letters.push_back('=');

        letter = prompt_option_loop(letters, "Enter letter corresponding to the row "s + letter_range + " (/ to select another column)");
        if (letter == '/')
            continue;

        if (letter == '=')
        {
            save_board(board);
            continue;
        }

        y = pickable_y_tiles[letter - 'a'];

        break;
    }

    return {x, y};
}

/**
 * @brief Prompt the user to select one of highlighted tiles from the board.
 *
 * @param board instance of Board to prompt from
 * @return std::pair<int, int> coordinates of the selected tile
 */
std::pair<int, int> prompt_tile_highlighted(Board const &board)
{
    int x{};
    int y{};

    for (;;)
    {
        print_board(board);
        auto letters = letter_vec(board.map.get_width());
        auto letter_range = first_last(letters);
        letters.push_back('=');
        auto letter = prompt_option_loop(letters, "Enter letter corresponding to the column "s + letter_range);

        if (letter == '=')
        {
            save_board(board);
            continue;
        }

        x = letter - 'a';

        std::vector<int> pickable_y_tiles{};
        for (auto [hx, hy] : board.highlights)
        {
            if (hx == x)
            {
                pickable_y_tiles.push_back(hy);
            }
        }

        if (pickable_y_tiles.empty())
        {
            printf("No pickable tiles in that column!\n");
            continue;
        }

        print_board(board, x, pickable_y_tiles);
        letters = letter_vec(pickable_y_tiles.size());
        letter_range = first_last(letters);
        letters.push_back('/');
        letters.push_back('=');

        letter = prompt_option_loop(letters, "Enter letter corresponding to the row "s + letter_range + " (/ to select another column)"s);
        if (letter == '/')
            continue;

        if (letter == '=')
        {
            save_board(board);
            continue;
        }

        y = pickable_y_tiles[letter - 'a'];

        break;
    }

    return {x, y};
}

/**
 * @brief The main game loop.
 *
 * @param board instance of Board to play on
 */
void game_loop(Board &board)
{
    for (;;)
    {
        printf("This is %s%s%s's turn!\n",
               board.current_player == Player::Ruby ? Ansi{}.bold().red().c_str() : Ansi{}.bold().white().c_str(),
               board.current_player == Player::Ruby ? "Ruby" : "Pearl",
               Ansi{}.reset().c_str());

        if (board.with_computer && board.current_player == Player::Pearl)
        {
            board.clear_highlights();

            print_board(board);

            const auto move = board.ai_play();

            board.selected_tile = move.from;
            const auto [to_x, to_y] = move.to;

            printf("AI moved {%d, %d} -> {%d, %d}\n", move.from.first, move.from.second, move.to.first, move.to.second);

            if (board.try_move(to_x, to_y))
            {
                board.next_player();
            }
        }
        else
        {
            board.clear_highlights();
            const auto [from_x, from_y] = prompt_tile_from(board);
            board.highlight_moves(from_x, from_y);
            board.selected_tile = {from_x, from_y};

            printf("Select the tile you want to move to:\n");
            const auto [to_x, to_y] = prompt_tile_highlighted(board);

            if (board.try_move(to_x, to_y))
            {
                board.next_player();
            }
        }

        if (board.game_ended())
        {
            board.clear_highlights();
            print_board(board);
            printf("Game over!\n");

            if (board.ruby_score > board.pearl_score)
            {
                printf("Ruby won!\n");
            }
            else if (board.ruby_score < board.pearl_score)
            {
                printf("Pearl won!\n");
            }
            else
            {
                printf("It's a draw!\n");
            }

            HighScoreManager{}.add_score(board.ruby_score, board.pearl_score);

            break;
        }
    }
}

int main()
{
    bool running = true;

    while (running)
    {
        printf("-= Hexxagon =-\n\n");

        printf("1 - Player vs Computer\n");
        printf("2 - Player vs Player\n");
        printf("3 - Load game from file\n");
        printf("0 - Exit\n");

        HighScoreManager high_score_manager{};
        if (!high_score_manager.scores.empty())
        {
            printf("\nHigh scores:\n");
            for (auto const &score : high_score_manager.scores)
            {
                auto outcome = score.winner == HighScoreManager::Winner::Ruby    ? "Ruby won"
                               : score.winner == HighScoreManager::Winner::Pearl ? "Pearl won"
                                                                                 : "Draw";
                printf("Ruby %d - %d Pearl, %s\n", score.ruby, score.pearl, outcome);
            }
        }

        printf("\n");
        printf("Enter '=' in game to save the game state to file.\n");

        auto option = prompt_option_loop(std::array{1, 2, 3, 0}, "Select an option"s);
        switch (option)
        {
        case 1:
        {
            Board board{};
            board.with_computer = true;
            board.reset(HexMap<TileState>{LEVEL1_TEMPLATE});
            game_loop(board);
            break;
        }
        case 2:
        {
            Board board{};
            board.with_computer = false;
            board.reset(HexMap<TileState>{LEVEL1_TEMPLATE});
            game_loop(board);
            break;
        }
        case 3:
        {
            std::string filename;
            printf("Enter filename or nothing to cancel: ");
            std::cin >> filename;

            if (filename.empty())
            {
                break;
            }

            Board board{};
            try
            {
                auto data = read_file(filename);
                board.deserialize(data);
            }
            catch (std::exception const &e)
            {
                printf("Error loading game: %s\n", e.what());
                break;
            }

            game_loop(board);
            break;
        }
        case 0:
            running = false;
            break;
        }
    }

    return 0;
}