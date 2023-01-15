#pragma once

#include "scene.h"
#include <common/board.h>

namespace hexx::gui
{
    /**
     * @brief The game scene
     *
     * @see SceneBase for information about each method
     */
    class SceneGame : public SceneBase
    {
        std::pair<int, int> clicked_tile{-1, -1};

    public:
        common::Board board;

        SceneGame();

        virtual ~SceneGame() = default;

        void tick(Context &ctx) override;

        void draw(Context &ctx) override;

        void mouse_down(Context &ctx, uint8_t index, int x, int y) override;

        void mouse_up(Context &ctx, uint8_t index, int x, int y) override;

        void mouse_move(Context &ctx, int x, int y) override;

        void key_down(Context &ctx, int key) override;
    };
};