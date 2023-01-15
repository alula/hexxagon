#pragma once

#include <span>
#include <cstdint>

#include "render.h"

namespace hexx::gui
{
    extern std::span<uint8_t> resource_sprites_bmp;

    /**
     * @brief Stores resources used by the game.
     */
    class ResourceInstances
    {
        std::shared_ptr<Texture> sprites_texture;

    public:
        Sprite hexagon;
        Sprite highlight;
        Sprite ruby;
        Sprite pearl;
        Sprite logo_hexx;
        Sprite logo_agon;

        TextRenderer font;

        ResourceInstances(SDL_Renderer *renderer) : sprites_texture(std::make_shared<Texture>()),
                                                    hexagon(sprites_texture, {0, 0, 40, 32}),
                                                    highlight(sprites_texture, {0, 32, 48, 32}),
                                                    ruby(sprites_texture, {64, 0, 32, 32}),
                                                    pearl(sprites_texture, {96, 0, 32, 32}),
                                                    logo_hexx(sprites_texture, {48, 32, 48, 9}),
                                                    logo_agon(sprites_texture, {48, 41, 48, 9}),
                                                    font(sprites_texture, {.char_width = 6,
                                                                           .char_height = 10,
                                                                           .texture_x = 0,
                                                                           .texture_y = 64,
                                                                           .chars_per_line = 16})
        {
            sprites_texture->initialize_from_bmp_buffer(renderer, resource_sprites_bmp);
        }
    };
}