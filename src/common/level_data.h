#pragma once

#include "board.h"

// clang-format off
namespace hexx::common
{
    // - void
    // # empty
    // P pearl
    // R ruby
    // -   -   P   -   -
    //   -   #   #   -
    // -   #   #   #   -
    //   #   #   #   #
    // R   #   #   #   R
    //   #   #   #   #
    // #   #   -   #   #
    //   #   #   #   #
    // #   #   #   #   #
    //   #   -   -   #
    // #   #   #   #   #
    //   #   #   #   #
    // P   #   #   #   P
    //   #   #   #   #
    // -   #   #   #   -
    //   -   #   #   -
    // -   -   R   -   -
    const HexMap<TileState> LEVEL1_TEMPLATE{
        9, 
        9, 
    {
        TileState::Void, TileState::Void, TileState::Void, TileState::Empty, TileState::Pearl, TileState::Empty, TileState::Void, TileState::Void, TileState::Void,
        TileState::Void, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Void,
        TileState::Ruby, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Ruby,
        TileState::Empty, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Void, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Empty,
        TileState::Empty, TileState::Empty, TileState::Empty, TileState::Void, TileState::Empty, TileState::Void, TileState::Empty, TileState::Empty, TileState::Empty,
        TileState::Empty, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Empty,
        TileState::Pearl, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Pearl,
        TileState::Void, TileState::Void, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Empty, TileState::Void, TileState::Void,
        TileState::Void, TileState::Void, TileState::Void, TileState::Void, TileState::Ruby, TileState::Void, TileState::Void, TileState::Void, TileState::Void
    }};
};
// clang-format on