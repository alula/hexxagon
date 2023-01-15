#pragma once

#include "scene.h"
#include "button.h"

namespace hexx::gui
{
    /**
     * @brief The game over scene
     * 
     * @see SceneBase for information about each method
     */
    class SceneGameOver : public SceneBase
    {
        Button ok_button{};
        int ruby_score{};
        int pearl_score{};

    public:
        SceneGameOver(int ruby_score, int pearl_score);

        virtual ~SceneGameOver() = default;

        void tick(Context &ctx) override;

        void draw(Context &ctx) override;

        void mouse_down(Context &ctx, uint8_t index, int x, int y) override;

        void mouse_up(Context &ctx, uint8_t index, int x, int y) override;

        void mouse_move(Context &ctx, int x, int y) override;
    };
}