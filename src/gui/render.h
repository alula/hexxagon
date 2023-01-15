#pragma once

#include <span>
#include <cstdint>
#include <memory>
#include <string>

#include "types.h"

extern "C"
{
    struct SDL_Renderer;
    struct SDL_Texture;
}

namespace hexx::gui
{
    class Sprite;
    class TextRenderer;
    class TextBuilder;

    enum class BlendMode
    {
        Alpha,
        Add
    };

    /**
     * @brief Represents a color in RGBA format.
     */
    struct Color
    {
        uint8_t r{};
        uint8_t g{};
        uint8_t b{};
        uint8_t a{};

        explicit constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}
        explicit constexpr Color(uint32_t rgba) : r(rgba & 0xFF),
                                                  g((rgba >> 8) & 0xFF),
                                                  b((rgba >> 16) & 0xFF),
                                                  a((rgba >> 24) & 0xFF)
        {
        }

        /**
         * @brief Create a Color object from HSL values.
         *
         * @param h Hue in degrees (0-360).
         * @param s Saturation in 0.0-1.0 range.
         * @param l Lightness in 0.0-1.0 range.
         * @return Color
         */
        static Color from_hsl(float h, float s, float l, float a = 1.0f);
    };

    static constexpr auto COLOR_RUBY = Color(232, 106, 115);
    static constexpr auto COLOR_PEARL = Color(218, 225, 234);

    class Texture
    {
        friend class Sprite;
        friend class TextRenderer;

        SDL_Texture *inner{nullptr};
        SDL_Renderer *renderer{nullptr};

        void dispose();

    public:
        Texture() {}
        ~Texture()
        {
            dispose();
        }

        /**
         * @brief Get the inner SDL texture instance.
         *
         * @return SDL_Texture* inner SDL texture instance.
         */
        SDL_Texture *get_inner() const
        {
            return inner;
        }

        /**
         * @brief Initializes this texture as render target.
         *
         * @param width width of the texture
         * @param height height of the texture
         */
        void initialize_render_target(SDL_Renderer *renderer, int width, int height);

        /**
         * @brief Initializes the texture from a .bmp file, provided as in-memory buffer.
         *
         * @param renderer instance of SDL renderer.
         * @param data buffer containing the .bmp file.
         */
        void initialize_from_bmp_buffer(SDL_Renderer *renderer, std::span<uint8_t> const &data);

        /**
         * @brief Draws a part of the texture in specified rectangle.
         *
         * @param src_rect the source rectangle
         * @param dst_rect the destination rectangle
         * @param blend_mode the blending mode to use
         */
        void draw(Rect const &src_rect, Rect const &dst_rect, BlendMode blend_mode = BlendMode::Alpha) const;
    };

    /**
     * @brief Represents a sprite and provides methods to draw it.
     */
    class Sprite
    {
        std::shared_ptr<Texture> texture;
        Rect rect;

    public:
        explicit Sprite(std::shared_ptr<Texture> const &texture, Rect const &rect) : texture(texture), rect(rect) {}

        /**
         * @brief Get the texture instance bound to this sprite.
         *
         * @return The texture of the sprite.
         */
        std::shared_ptr<Texture> get_texture() const
        {
            return texture;
        }

        /**
         * @brief Get the rectangle of the sprite.
         *
         * @return The rectangle of the sprite.
         */
        Rect const &get_rect() const
        {
            return rect;
        }

        /**
         * @brief Draw the sprite at the given position. The position is based on the top left corner of the sprite.
         *
         * @param x position on the x axis.
         * @param y position on the y axis.
         * @param blend_mode the blending mode to use.
         */
        void draw(int x, int y, BlendMode blend_mode = BlendMode::Alpha) const
        {
            texture->draw(rect, rect.with_pos(x, y), blend_mode);
        }

        /**
         * @brief Draw the sprite at the given position and size. The position is based on the center of the sprite.
         *
         * @param x position on the x axis.
         * @param y position on the y axis.
         * @param width width of the sprite.
         * @param height height of the sprite.
         * @param blend_mode the blending mode to use.
         */
        void draw_scaled(int x, int y, int width, int height, BlendMode blend_mode = BlendMode::Alpha) const
        {
            texture->draw(rect, {x, y, width, height}, blend_mode);
        }
    };

    /**
     * @brief Metadata of the font texture.
     */
    struct FontInfo
    {
        int char_width;
        int char_height;
        int texture_x;
        int texture_y;
        int chars_per_line;
    };

    /**
     * @brief A simple bitmap text renderer.
     */
    class TextRenderer
    {
        std::shared_ptr<Texture> texture;
        FontInfo font_info;

    public:
        explicit TextRenderer(std::shared_ptr<Texture> const &texture, FontInfo const &info) : texture(texture), font_info(info) {}

        /**
         * @return TextBuilder instance to build a text to draw.
         */
        TextBuilder builder() const;

        void draw_text(int x, int y, std::string const &text, Color const &color) const;

        /**
         * @brief Calculates the size of the given text.
         *
         * @param text the text to calculate the size of.
         * @return std::pair<int, int> the size of the text.
         */
        std::pair<int, int> get_text_size(std::string const &text) const
        {
            return {font_info.char_width * text.size(), font_info.char_height};
        }
    };

    class TextBuilder
    {
        TextRenderer const &rend;
        int x{0};
        int y{0};
        std::string text{};
        Color color{255, 255, 255, 255};

    public:
        TextBuilder(TextRenderer const &rend) : rend(rend) {}

        /**
         * @brief Sets the position of the text.
         *
         * @param x x position
         * @param y y position
         * @return TextBuilder&
         */
        TextBuilder &with_pos(int x, int y)
        {
            this->x = x;
            this->y = y;
            return *this;
        }

        /**
         * @brief Sets the text to draw.
         *
         * @param text the text to draw
         * @return TextBuilder&
         */
        TextBuilder &with_text(std::string const &text)
        {
            this->text = text;
            return *this;
        }

        /**
         * @brief Sets the text to draw.
         *
         * @param text the text to draw
         * @return TextBuilder&
         */
        TextBuilder &with_text(std::string &&text)
        {
            this->text = std::move(text);
            return *this;
        }

        /**
         * @brief Sets the color of the text.
         *
         * @param color the color of the text
         * @return TextBuilder&
         */
        TextBuilder &with_color(Color const &color)
        {
            this->color = color;
            return *this;
        }

        /**
         * @brief Draws the text.
         */
        void draw() const
        {
            rend.draw_text(x, y, text, color);
        }
    };

    /**
     * @brief Renders a filled rectangle with the given color.
     *
     * @param renderer instance of SDL renderer.
     * @param rect the rectangle to fill.
     * @param color the color to fill the rectangle with.
     */
    void fill_rect(SDL_Renderer *renderer, Rect const &rect, Color const &color);
}