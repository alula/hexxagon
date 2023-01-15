#pragma once

#include "hexmap.h"

#include <utility>
#include <vector>

namespace hexx::common
{
    /**
     * @brief Enumeration for the state of a tile on the game board.
     * This enumeration is used to represent the state of a tile on the game board. Each value represents a different state:
     *
     * - Void: ignored, not a part of the board
     * - Empty: not occupied by any gem
     * - Ruby: occupied by ruby
     * - Pearl: occupied by pearl
     */
    enum class TileState
    {
        Void,
        Empty,
        Ruby,
        Pearl
    };

    /**
     * @brief This enumeration is used to represent the two players in the game.
     */
    enum class Player
    {
        Ruby,
        Pearl,
    };

    /**
     * @brief Struct for storing information about a move made by a player.
     * This struct contains information about a move made by a player, including the start and end positions.
     */
    struct MoveInfo
    {
        std::pair<int, int> from;
        std::pair<int, int> to;
    };

    /**
     * @brief Represents the state of the game board.
     */
    class Board
    {
        void update_score();

    public:
        HexMap<TileState> map{};
        Player current_player = Player::Ruby;
        std::vector<std::pair<int, int>> highlights;
        std::pair<int, int> selected_tile{-1, -1};
        int ruby_score{0};
        int pearl_score{0};
        bool with_computer{false};

        /**
         * @brief Resets the board to a new state.
         *
         * @param new_map starting level template
         */
        void reset(HexMap<TileState> &&new_map);

        /**
         * @brief Returns a vector containing possible moves starting from tile at specified coordinates.
         *
         * @param x X position of the tile.
         * @param y Y position of the tile.
         * @return std::vector<std::pair<int, int>> vector containing possible moves.
         */
        std::vector<std::pair<int, int>> get_possible_moves(int x, int y) const;

        /**
         * @brief Highlight tiles the player can move to starting from the specified tile.
         */
        void highlight_moves(int x, int y);

        /**
         * @brief Clears the tile highlights.
         */
        void clear_highlights();

        /**
         * @brief Tries to move the player selected tile to the specified tile.
         *
         * @return true if the move was successful
         */
        bool try_move(int x, int y);

        /**
         * @brief Checks if the current player can move.
         *
         * @return true if the player can move
         */
        bool can_move() const;

        /**
         * @brief Checks whether the game has ended.
         *
         * @return true if game has ended.
         */
        bool game_ended() const;

        /**
         * @brief Gives the turn to next player who can make a move.
         *
         */
        void next_player();

        /**
         * @brief This function is used to compute the next move for the AI player.
         * The AI player will try to find a move that will take over the most gems.
         * If no such move can be found, it will pick a random move.
         *
         * @return MoveInfo  a struct containing the start and end positions of the move determined by the AI.
         */
        MoveInfo ai_play();

        /**
         * @brief Serializes the board state into a vector of bytes.
         *
         * @return std::vector<uint8_t> vector of bytes representing the board state.
         */
        std::vector<uint8_t> serialize() const;

        /**
         * @brief Deserializes the board state from a vector of bytes.
         *
         * @param data vector of bytes representing the board state.
         */
        void deserialize(std::vector<uint8_t> const &data);
    };
}