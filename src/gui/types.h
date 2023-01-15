#pragma once

namespace hexx::gui
{
    struct Rect
    {
        int x{};
        int y{};
        int w{};
        int h{};

        Rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}

        Rect with_pos(int x, int y) const
        {
            return Rect{x, y, w, h};
        }

        constexpr bool contains(int x, int y) const
        {
            return x >= this->x && x < this->x + w && y >= this->y && y < this->y + h;
        }
    };
}