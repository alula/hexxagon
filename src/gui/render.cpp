#include "render.h"

#include <stdexcept>
#include <cmath>

#include <SDL.h>

using namespace hexx::gui;

Color Color::from_hsl(float h, float s, float l, float a)
{
    h = std::fmod(h, 360.0f);
    float c = (1 - std::abs(2 * l - 1)) * s;
    float x = c * (1 - std::abs(std::fmod(h / 60.0f, 2) - 1));
    float m = l - c / 2;

    float r, g, b;

    if (h < 60)
    {
        r = c;
        g = x;
        b = 0;
    }
    else if (h < 120)
    {
        r = x;
        g = c;
        b = 0;
    }
    else if (h < 180)
    {
        r = 0;
        g = c;
        b = x;
    }
    else if (h < 240)
    {
        r = 0;
        g = x;
        b = c;
    }
    else if (h < 300)
    {
        r = x;
        g = 0;
        b = c;
    }
    else
    {
        r = c;
        g = 0;
        b = x;
    }

    return Color{static_cast<uint8_t>((r + m) * 255.0f),
                 static_cast<uint8_t>((g + m) * 255.0f),
                 static_cast<uint8_t>((b + m) * 255.0f),
                 static_cast<uint8_t>(a * 255.0f)};
}

void Texture::dispose()
{
    if (inner != nullptr)
    {
        SDL_DestroyTexture(inner);
        inner = nullptr;
    }
}

void Texture::initialize_render_target(SDL_Renderer *renderer, int width, int height)
{
    dispose();

    this->renderer = renderer;

    inner = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);

    if (!inner)
    {
        throw std::runtime_error("failed to create texture: " + std::string(SDL_GetError()));
    }
}

void Texture::initialize_from_bmp_buffer(SDL_Renderer *renderer, std::span<uint8_t> const &data)
{
    dispose();

    this->renderer = renderer;

    auto bitmap = SDL_LoadBMP_RW(SDL_RWFromConstMem(data.data(), data.size()), 1);
    if (!bitmap)
    {
        throw std::runtime_error("failed to load specified bitmap.");
    }

    inner = SDL_CreateTextureFromSurface(renderer, bitmap);
    SDL_FreeSurface(bitmap);

    if (!inner)
    {
        throw std::runtime_error("failed to create texture: " + std::string(SDL_GetError()));
    }
}

static constexpr SDL_Rect to_sdl_rect(Rect const &rect)
{
    return SDL_Rect{rect.x, rect.y, rect.w, rect.h};
}

static constexpr SDL_BlendMode to_sdl_blend_mode(BlendMode blend_mode)
{
    switch (blend_mode)
    {
    case BlendMode::Alpha:
        return SDL_BLENDMODE_BLEND;
    case BlendMode::Add:
        return SDL_BLENDMODE_ADD;
    }
}

void Texture::draw(Rect const &src_rect, Rect const &dest_rect, BlendMode blend_mode) const
{
    if (!inner)
        return;

    const auto sdl_src = to_sdl_rect(src_rect);
    const auto sdl_drc = to_sdl_rect(dest_rect);

    SDL_SetTextureBlendMode(inner, to_sdl_blend_mode(blend_mode));
    SDL_RenderCopy(renderer, inner, &sdl_src, &sdl_drc);
}

void TextRenderer::draw_text(int x, int y, std::string const &text, Color const &color) const
{
    auto pos_x = x;
    auto pos_y = y;
    SDL_Rect src_rect{0, 0, font_info.char_width, font_info.char_height};
    SDL_Rect dst_rect{0, 0, font_info.char_width, font_info.char_height};

    SDL_SetTextureBlendMode(texture->inner, SDL_BLENDMODE_BLEND);
    SDL_SetTextureColorMod(texture->inner, color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(texture->inner, color.a);

    for (auto c : text)
    {
        if (c == '\n')
        {
            pos_x = x;
            pos_y += font_info.char_height;
        }
        else if (c <= ' ')
        {
            pos_x += font_info.char_width;
        }
        else
        {
            int char_idx = c - ' ';
            int char_x = char_idx % font_info.chars_per_line;
            int char_y = char_idx / font_info.chars_per_line;

            src_rect.x = font_info.texture_x + font_info.char_width * char_x;
            src_rect.y = font_info.texture_y + font_info.char_height * char_y;

            dst_rect.x = pos_x;
            dst_rect.y = pos_y;

            SDL_RenderCopy(texture->renderer, texture->inner, &src_rect, &dst_rect);

            pos_x += font_info.char_width;
        }
    }

    SDL_SetTextureColorMod(texture->inner, 255, 255, 255);
    SDL_SetTextureAlphaMod(texture->inner, 255);
}

TextBuilder TextRenderer::builder() const
{
    return TextBuilder(*this);
}

void hexx::gui::fill_rect(SDL_Renderer *renderer, Rect const &rect, Color const &color)
{
    const auto sdl_rect = to_sdl_rect(rect);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &sdl_rect);
}