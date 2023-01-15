#pragma once

#include <vector>
#include <memory>

#include "render.h"

extern "C"
{
    struct SDL_Renderer;
}

namespace hexx::gui
{
    static constexpr int DISPLAY_WIDTH = 400;
    static constexpr int DISPLAY_HEIGHT = 300;
    static constexpr int DISPLAY_SCALE = 2;

    struct Context;

    /**
     * @brief The base class for all scenes in the application.
     * This class defines the interface that all scenes in the application must implement
     * and contains internal state used by the manager.
     * 
     * @see SceneManager for information about each method.
     */
    class SceneBase
    {
    public:
        /**
         * @brief This is null until added to scene manager.
         */
        std::shared_ptr<Texture> back_buffer{nullptr};

        virtual ~SceneBase() {}

        virtual void tick(Context &ctx) = 0;

        virtual void draw(Context &ctx) = 0;

        virtual void key_down(Context &ctx, int key) {}

        virtual void key_up(Context &ctx, int key) {}

        virtual void mouse_down(Context &ctx, uint8_t index, int x, int y) {}

        virtual void mouse_up(Context &ctx, uint8_t index, int x, int y) {}

        virtual void mouse_move(Context &ctx, int x, int y) {}

        virtual void text_input(Context &ctx, std::string const &text) {}
    };

    /**
     * @brief The manager class for all scenes in the application.
     * This class is responsible for maintaining the scene stack and updating and rendering the current scene.
     */
    class SceneManager
    {
        std::vector<std::unique_ptr<SceneBase>> scene_stack{};
        SDL_Renderer *renderer;

    public:
        SceneManager(SDL_Renderer *renderer) : renderer(renderer) {}

        /**
         * @brief Update the state of the current scene.
         *
         * @param ctx The context of the current scene.
         */
        void tick(Context &ctx);

        /**
         * @brief Render the current scene.
         *
         * @param ctx The context of the current scene.
         */
        void draw(Context &ctx);

        /**
         * @brief Handle key down events.
         *
         * @param ctx The context of the current scene.
         * @param key The key that was pressed.
         */
        void key_down(Context &ctx, int key);

        /**
         * @brief Handle key up events.
         *
         * @param ctx The context of the current scene.
         * @param key The key that was released.
         */
        void key_up(Context &ctx, int key);

        /**
         * @brief Handle mouse down events.
         *
         * @param ctx The context of the current scene.
         * @param index The index of the mouse button that was pressed.
         * @param x The x coordinate of the mouse.
         * @param y The y coordinate of the mouse.
         */
        void mouse_down(Context &ctx, uint8_t index, int x, int y);

        /**
         * @brief Handle mouse up events.
         *
         * @param ctx The context of the current scene.
         * @param index The index of the mouse button that was released.
         * @param x The x coordinate of the mouse.
         * @param y The y coordinate of the mouse.
         */
        void mouse_up(Context &ctx, uint8_t index, int x, int y);

        /**
         * @brief Handle mouse move events.
         *
         * @param ctx The context of the current scene.
         * @param x The x coordinate of the mouse.
         * @param y The y coordinate of the mouse.
         */
        void mouse_move(Context &ctx, int x, int y);

        /**
         * @brief Handle text input events.
         *
         * @param ctx The context of the current scene.
         * @param text The text that was input.
         */
        void text_input(Context &ctx, std::string const &text);

        /**
         * @brief Push a scene onto the scene stack.
         *
         * @param scene The scene to be pushed onto the stack.
         */
        void push(std::unique_ptr<SceneBase> &&scene);

        /**
         * @brief Replace the current scene with a new scene.
         *
         * @param scene The scene to replace the current scene with.
         */
        void replace(std::unique_ptr<SceneBase> &&scene);

        /**
         * @brief Pop the current scene off the scene stack.
         */
        void pop();
    };
};