#pragma once

#include <cstdint>
#include <chrono>
#include <functional>
#include <thread>

namespace hexx::gui
{
    /**
     * @brief Represents and runs the game loop with a fixed timestep
     */
    struct GameLoop
    {
        static constexpr uint64_t target_fps = 60ULL;
        static constexpr uint64_t delta = 1000000000ULL / target_fps;

        uint64_t start_tick{};
        uint64_t next_tick{};
        uint64_t last_tick{};

        unsigned int loops{};

        /**
         * @brief Runs the game loop
         *
         * @param callback The callback to call each tick. If the callback returns false, the loop will stop.
         */
        void run(std::function<bool()> callback)
        {
            for (;;)
            {
                auto lt = next_tick;
                while ((get_nano_monotonic() - start_tick) >= next_tick && loops < 3)
                {
                    next_tick += delta;
                    loops++;
                }

                if (loops == 3)
                {
                    // reset if paused for too long
                    last_tick = (get_nano_monotonic() - start_tick);
                    next_tick = last_tick + delta;
                    loops = 0;
                }

                if (loops != 0)
                {
                    last_tick = lt;
                }
                else
                {
                    std::this_thread::sleep_for(std::chrono::microseconds(1000));
                }

                for (auto i = 0u; i < loops; i++)
                {
                    if (!callback())
                        return;
                }

                loops = 0;
            }
        }

        /**
         * @brief eturns the current timestamp in nanoseconds using monotonic clock.
         *
         * @return uint64_t timestamp value
         */
        static inline uint64_t get_nano_monotonic()
        {
            using nano_unsigned = std::chrono::duration<uint64_t, std::nano>;

            return std::chrono::duration_cast<nano_unsigned>(std::chrono::steady_clock::now().time_since_epoch()).count();
        }
    };

};