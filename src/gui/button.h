#pragma once

#include <functional>
#include <string>

#include "context.h"

namespace hexx::gui
{
    /**
     * @brief A simple button class.
     * @see SceneManager for description of the tick, draw, mouse_down, mouse_up and mouse_move methods.
     */
    class Button
    {
        std::function<void(Context &)> callback{};
        std::string text{};
        Rect rect{0, 0, 0, 0};
        bool hovered{false};
        bool pressed{false};

    public:
        Button();

        void tick(Context &ctx);

        void draw(Context &ctx);

        /**
         * @brief Set the text of the button.
         * @param text The text to set.
         */
        void set_text(std::string &&text)
        {
            this->text = std::move(text);
        }

        /**
         * @brief Set the text of the button.
         * @param text The text to set.
         */
        void set_text(std::string const &text)
        {
            this->text = text;
        }

        /**
         * @brief Set the callback of the button.
         * @param callback The callback to set.
         */
        void set_callback(std::function<void(Context &)> &&callback)
        {
            this->callback = std::move(callback);
        }

        /**
         * @brief Set the callback of the button.
         * @param callback The callback to set.
         */
        void set_rect(Rect const &rect)
        {
            this->rect = rect;
        }

        void mouse_down(Context &ctx, int x, int y);

        void mouse_up(Context &ctx, int x, int y);

        void mouse_move(Context &ctx, int x, int y);
    };
}