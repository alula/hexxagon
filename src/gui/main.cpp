#include "scene.h"
#include "render.h"
#include "resources.h"
#include "context.h"
#include "game_loop.h"
#include "scene_mainmenu.h"

#include <SDL.h>

#include <string>
#include <memory>
#include <chrono>

using namespace hexx::gui;

using std::operator""s;

[[noreturn]] static inline void bailout(std::string const &msg)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", msg.c_str(), NULL);
    exit(1);
}

void Context::message_box(std::string const &message)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Hexxagon", message.c_str(), NULL);
}

#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main()
#endif
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        bailout("Failed to initialize SDL.");
    }

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

    const auto win = SDL_CreateWindow("Hexxagon",
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      DISPLAY_WIDTH * DISPLAY_SCALE, DISPLAY_HEIGHT * DISPLAY_SCALE,
                                      SDL_WINDOW_SHOWN);
    const auto render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    if (!win)
    {
        bailout("Failed to create a window: "s + std::string(SDL_GetError()));
    }

    if (!render)
    {
        bailout("Failed to create a renderer: "s + std::string(SDL_GetError()));
    }

    SDL_RenderSetIntegerScale(render, SDL_TRUE);
    SDL_RenderSetScale(render, DISPLAY_SCALE, DISPLAY_SCALE);

    Context ctx{render};
    GameLoop game_loop{};

    auto fps = 0;
    auto last_fps = 0;
    auto last_fps_tick = 0;

    ctx.manager.push(std::make_unique<SceneMainMenu>());

    game_loop.run(
        [&]()
        {
            if (SDL_GetTicks() - last_fps_tick >= 1000)
            {
                last_fps = fps;
                last_fps_tick = SDL_GetTicks();
                fps = 0;
            }

            fps++;

            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_QUIT:
                {
                    ctx.running = false;
                    break;
                }

                case SDL_KEYDOWN:
                {
                    if (event.key.keysym.scancode == SDL_SCANCODE_F12)
                        ctx.debug = !ctx.debug;
                    ctx.manager.key_down(ctx, event.key.keysym.sym);
                    break;
                }

                case SDL_KEYUP:
                {
                    ctx.manager.key_up(ctx, event.key.keysym.sym);
                    break;
                }

                case SDL_MOUSEBUTTONDOWN:
                {
                    ctx.manager.mouse_down(ctx, event.button.button, event.button.x / DISPLAY_SCALE, event.button.y / DISPLAY_SCALE);
                    break;
                }

                case SDL_MOUSEBUTTONUP:
                {
                    ctx.manager.mouse_up(ctx, event.button.button, event.button.x / DISPLAY_SCALE, event.button.y / DISPLAY_SCALE);
                    break;
                }

                case SDL_MOUSEMOTION:
                {
                    ctx.manager.mouse_move(ctx, event.motion.x / DISPLAY_SCALE, event.motion.y / DISPLAY_SCALE);
                    break;
                }

                case SDL_TEXTINPUT:
                {
                    ctx.manager.text_input(ctx, std::string(event.text.text));
                    break;
                }

                default:
                    break;
                }
            }

            SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
            SDL_RenderClear(render);

            ctx.manager.tick(ctx);
            ctx.manager.draw(ctx);

            if (ctx.debug)
            {
                ctx.res.font.builder().with_pos(2, 2).with_color(Color(0, 255, 0)).with_text("fps: " + std::to_string(last_fps)).draw();
            }

            SDL_RenderPresent(render);

            return ctx.running;
        });

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}