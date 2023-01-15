#pragma once

#include <functional>
#include <list>

namespace hexx::common
{
    /**
     * @brief An utility class that tracks game frames and invokes queued callbacks on specified frames.
     */
    class Sequencer
    {
    public:
        struct CallbackInfo
        {
            /**
             * @brief number of the first frame on which this callback should be invoked.
             */
            long start_tick;

            /**
             * @brief The animation callback. First parameter is the delta of frames since start_tick,
             * return value indicates whether the callback should be queued for the next frame or removed.
             */
            std::function<bool(long)> callback;
        };

        long tick{0};
        std::list<CallbackInfo> callbacks;

        /**
         * @brief Advances to next frame.
         * @return true if there's no pending callbacks.
         */
        bool advance();
    };
};