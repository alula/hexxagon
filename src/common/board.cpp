#include "board.h"
#include "rng.h"
#include "byte_utils.h"

#include <ranges>
#include <algorithm>
#include <array>
#include <tuple>

using namespace hexx::common;

void Board::update_score()
{
    ruby_score = 0;
    pearl_score = 0;

    for (auto tile = map.cbegin(); tile != map.cend(); tile++)
    {
        if (*tile == TileState::Ruby)
        {
            ruby_score++;
        }
        else if (*tile == TileState::Pearl)
        {
            pearl_score++;
        }
    }
}

void Board::reset(HexMap<TileState> &&new_map)
{
    map = std::move(new_map);
    current_player = Player::Ruby;
    highlights.clear();
    selected_tile = {-1, -1};
    update_score();
}

constexpr std::array<std::tuple<int, int, int>, 6> direction_vectors = {
    // {q, r, s}
    // first layer
    std::tuple{0, -1, 1},
    {1, -1, 0},
    {1, 0, -1},
    {0, 1, -1},
    {-1, 1, 0},
    {-1, 0, 1},
};

constexpr std::array<std::tuple<int, int, int>, 18> direction_vectors_two_layer = {
    // {q, r, s}
    // first layer
    std::tuple{0, -1, 1},
    {1, -1, 0},
    {1, 0, -1},
    {0, 1, -1},
    {-1, 1, 0},
    {-1, 0, 1},

    // second layer
    {0, -2, 2},
    {1, -2, 1},
    {2, -2, 0},
    {2, -1, -1},
    {2, 0, -2},
    {1, 1, -2},
    {0, 2, -2},
    {-1, 2, -1},
    {-2, 2, 0},
    {-2, 1, 1},
    {-2, 0, 2},
    {-1, -1, 2},
};

std::vector<std::pair<int, int>> Board::get_possible_moves(int x, int y) const
{
    const auto players_tile_type = current_player == Player::Ruby ? TileState::Ruby : TileState::Pearl;
    const auto starting_tile = map.at(x, y);

    if (starting_tile == map.cend() || *starting_tile != players_tile_type)
    {
        return {};
    }

    std::vector<std::pair<int, int>> moves;

    for (const auto [dq, dr, ds] : direction_vectors_two_layer)
    {
        auto tile_off = starting_tile.relative_cube(dq, dr, ds);
        if (tile_off == map.cend())
            continue;

        // printf("tile_off: %d %d %d -> %d, %d\n", dq, dr, ds, tile_off.face_x(), tile_off.face_y());

        if (*tile_off == TileState::Empty)
        {
            moves.push_back({tile_off.face_x(), tile_off.face_y()});
        }
    }

    return moves;
}

void Board::highlight_moves(int x, int y)
{
    clear_highlights();

    for (const auto [dx, dy] : get_possible_moves(x, y))
    {
        highlights.push_back({dx, dy});
    }
}

void Board::clear_highlights()
{
    highlights.clear();
}

bool Board::try_move(int x, int y)
{
    auto starting_tile = map.at(selected_tile.first, selected_tile.second);

    const auto players_tile_type = current_player == Player::Ruby ? TileState::Ruby : TileState::Pearl;
    const auto opponent_tile_type = current_player == Player::Ruby ? TileState::Pearl : TileState::Ruby;

    if (starting_tile == map.end())
    {
        return false;
    }

    if (*starting_tile != players_tile_type)
    {
        return false;
    }

    auto target_tile = map.at(x, y);
    if (*target_tile != TileState::Empty)
    {
        return false;
    }

    const auto distance = starting_tile.distance_cube(target_tile);

    // if we hop to neighboring cell, the gem gets cloned.
    if (distance == 1)
    {
        *target_tile = players_tile_type;
    }
    // otherwise we move the gem from one cell to another
    else if (distance == 2)
    {
        *starting_tile = TileState::Empty;
        *target_tile = players_tile_type;
    }
    else
    {
        return false;
    }

    for (const auto [dq, dr, ds] : direction_vectors)
    {
        // change all opponent's gems around our target to our gems
        auto iter = target_tile.relative_cube(dq, dr, ds);
        if (iter == map.end())
            continue;

        if (*iter == opponent_tile_type)
        {
            *iter = players_tile_type;
        }
    }

    update_score();
    selected_tile = {-1, -1};

    return true;
}

bool Board::can_move() const
{
    for (auto tile = map.cbegin(); tile != map.cend(); tile++)
    {
        if (!get_possible_moves(tile.face_x(), tile.face_y()).empty())
            return true;
    }

    return false;
}

bool Board::game_ended() const
{
    return std::ranges::none_of(map, [](auto const &tile)
                                { return tile == TileState::Ruby; }) ||
           std::ranges::none_of(map, [](auto const &tile)
                                { return tile == TileState::Pearl; }) ||
           std::ranges::none_of(map, [](auto const &tile)
                                { return tile == TileState::Empty; });
}

void Board::next_player()
{
    if (game_ended())
        return;

    for (;;)
    {
        if (current_player == Player::Pearl)
            current_player = Player::Ruby;
        else if (current_player == Player::Ruby)
            current_player = Player::Pearl;

        if (can_move() || game_ended())
            break;
    }
}

MoveInfo Board::ai_play()
{
    // this could be expanded to chose "least risky" outcome but imo it's good enough for this project

    // find move that will take over the most gems
    const auto players_tile_type = current_player == Player::Ruby ? TileState::Ruby : TileState::Pearl;
    const auto opponent_tile_type = current_player == Player::Ruby ? TileState::Pearl : TileState::Ruby;

    std::pair<int, int> best_from{-1, -1};
    std::pair<int, int> best_to{-1, -1};
    int best_move_score = 0;

    for (auto tile = map.cbegin(); tile != map.cend(); tile++)
    {
        if (*tile != players_tile_type)
            continue;

        auto moves = get_possible_moves(tile.face_x(), tile.face_y());

        for (const auto [dx, dy] : moves)
        {
            auto target_tile = map.at(dx, dy);

            int move_score = 0;

            for (const auto [dq, dr, ds] : direction_vectors)
            {
                auto iter = target_tile.relative_cube(dq, dr, ds);
                if (iter == map.end())
                    continue;

                if (*iter == opponent_tile_type)
                {
                    move_score++;
                }
            }

            if (move_score > best_move_score)
            {
                best_move_score = move_score;
                best_from = {tile.face_x(), tile.face_y()};
                best_to = {dx, dy};
            }
        }
    }

    // pick a random move if we can't find a good one
    if (best_move_score == 0)
    {
        // >_< laziness moment
        std::vector<std::pair<std::pair<int, int>, std::vector<std::pair<int, int>>>> possible_moves;

        for (auto tile = map.cbegin(); tile != map.cend(); tile++)
        {
            if (*tile != players_tile_type)
                continue;
            auto moves = get_possible_moves(tile.face_x(), tile.face_y());

            if (!moves.empty())
            {
                possible_moves.push_back({{tile.face_x(), tile.face_y()}, std::move(moves)});
            }
        }

        if (possible_moves.empty())
        {
            throw std::runtime_error("there's no possible moves!");
        }

        auto &[pos_from, move_list] = random_choice(possible_moves);
        auto pos_to = random_choice(move_list);
        best_from = pos_from;
        best_to = pos_to;
    }

    return MoveInfo{
        .from = best_from,
        .to = best_to,
    };
}

constexpr static uint32_t MAGIC_NUMBER = 0x26306B0A;

std::vector<uint8_t> Board::serialize() const
{
    ByteWriter writer;
    writer.write_uint32(MAGIC_NUMBER);
    writer.write_uint16(1); // version
    writer.write_uint8(with_computer ? 1 : 0);
    writer.write_int32(map.get_width());
    writer.write_int32(map.get_height());
    for (auto y = 0; y < map.get_height(); y++)
    {
        for (auto x = 0; x < map.get_width(); x++)
        {
            writer.write_uint8(static_cast<uint8_t>(*map.at(x, y)));
        }
    }

    writer.write_uint8(static_cast<uint8_t>(current_player));
    writer.write_uint32(highlights.size());
    for (const auto &[x, y] : highlights)
    {
        writer.write_int32(x);
        writer.write_int32(y);
    }
    writer.write_int32(selected_tile.first);
    writer.write_int32(selected_tile.second);
    writer.write_uint32(ruby_score);
    writer.write_uint32(pearl_score);

    return writer.data;
}

void Board::deserialize(std::vector<uint8_t> const &data)
{
    ByteReader reader(data);
    auto magic_number = reader.read_uint32();
    if (magic_number != MAGIC_NUMBER)
    {
        throw std::runtime_error("invalid magic number");
    }

    auto version = reader.read_uint16();
    if (version != 1)
    {
        throw std::runtime_error("invalid version");
    }

    with_computer = reader.read_uint8() != 0;

    auto width = reader.read_int32();
    auto height = reader.read_int32();
    std::vector<TileState> tiles(width * height);
    tiles.resize(width * height, TileState::Empty);

    for (auto y = 0; y < height; y++)
    {
        for (auto x = 0; x < width; x++)
        {
            tiles[y * width + x] = static_cast<TileState>(reader.read_uint8());
        }
    }

    map = std::move(HexMap<TileState>(width, height, std::move(tiles)));

    current_player = static_cast<Player>(reader.read_uint8());
    auto highlights_size = reader.read_uint32();
    highlights.clear();

    for (auto i = 0; i < highlights_size; i++)
    {
        highlights.push_back({reader.read_int32(), reader.read_int32()});
    }

    selected_tile = {reader.read_int32(), reader.read_int32()};
    ruby_score = reader.read_uint32();
    pearl_score = reader.read_uint32();
}
