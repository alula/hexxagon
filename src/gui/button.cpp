#include "button.h"

#include <SDL.h>

using namespace hexx::gui;

Button::Button() {}

void Button::tick(Context &ctx)
{
}

void Button::draw(Context &ctx)
{
    auto color = Color(51, 57, 63);

    if (hovered)
    {
        color = Color(74, 84, 98);
    }

    if (pressed)
    {
        color = Color(66, 36, 51);
    }

    fill_rect(ctx.render, rect, color);

    auto [text_width, text_height] = ctx.res.font.get_text_size(text);

    ctx.res.font.builder()
        .with_pos(rect.x + (rect.w - text_width) / 2, rect.y + (rect.h - text_height) / 2)
        .with_text(text)
        .draw();
}

void Button::mouse_down(Context &ctx, int x, int y)
{
    if (rect.contains(x, y))
    {
        pressed = true;
    }
}

void Button::mouse_up(Context &ctx, int x, int y)
{
    if (rect.contains(x, y) && pressed)
    {
        callback(ctx);
    }

    pressed = false;
}

void Button::mouse_move(Context &ctx, int x, int y)
{
    hovered = rect.contains(x, y);
}