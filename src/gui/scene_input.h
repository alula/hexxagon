#pragma once

#include "scene.h"
#include "button.h"

#include <functional>

namespace hexx::gui
{
    /**
     * @brief The text input scene
     * 
     * @see SceneBase for information about each method
     */
    class SceneInput : public SceneBase
    {
        Button ok_button{};
        std::string message;
        std::string input;
        unsigned int pos;

        std::function<void(Context &, std::string)> callback;

    public:
        SceneInput(std::string message, std::function<void(Context &, std::string)> callback);

        virtual ~SceneInput() = default;

        void tick(Context &ctx) override;

        void draw(Context &ctx) override;

        void mouse_down(Context &ctx, uint8_t index, int x, int y) override;

        void mouse_up(Context &ctx, uint8_t index, int x, int y) override;

        void mouse_move(Context &ctx, int x, int y) override;

        void key_down(Context &ctx, int key) override;

        void text_input(Context &ctx, std::string const &text) override;
    };
}