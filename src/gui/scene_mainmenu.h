#pragma once

#include "scene.h"
#include "button.h"

namespace hexx::gui
{
    /**
     * @brief The main menu scene
     * 
     * @see SceneBase for information about each method
     */
    class SceneMainMenu : public SceneBase
    {
        Button single_player_button{};
        Button multi_player_button{};
        Button load_button{};
        Button quit_button{};

    public:
        SceneMainMenu();

        virtual ~SceneMainMenu() = default;

        void tick(Context &ctx) override;

        void draw(Context &ctx) override;

        void mouse_down(Context &ctx, uint8_t index, int x, int y) override;

        void mouse_up(Context &ctx, uint8_t index, int x, int y) override;

        void mouse_move(Context &ctx, int x, int y) override;
    };
}