#pragma once

#include <string>

namespace hexx::cli
{
    struct Ansi
    {
        static bool enable_color;

        std::string buf{};

        Ansi &bold()
        {
            if (enable_color)
                buf.append("\x1b[1m");
            return *this;
        }

        Ansi &reset()
        {
            if (enable_color)
                buf.append("\x1b[0m");
            return *this;
        }

        Ansi &red()
        {
            if (enable_color)
                buf.append("\x1b[31m");
            return *this;
        }

        Ansi &green()
        {
            if (enable_color)
                buf.append("\x1b[32m");
            return *this;
        }

        Ansi &yellow()
        {
            if (enable_color)
                buf.append("\x1b[33m");
            return *this;
        }

        Ansi &blue()
        {
            if (enable_color)
                buf.append("\x1b[34m");
            return *this;
        }

        Ansi &magenta()
        {
            if (enable_color)
                buf.append("\x1b[35m");
            return *this;
        }

        Ansi &cyan()
        {
            if (enable_color)
                buf.append("\x1b[36m");
            return *this;
        }

        Ansi &white()
        {
            if (enable_color)
                buf.append("\x1b[37m");
            return *this;
        }

        Ansi &black()
        {
            if (enable_color)
                buf.append("\x1b[30m");
            return *this;
        }

        const char *c_str() const
        {
            return buf.c_str();
        }
    };

}