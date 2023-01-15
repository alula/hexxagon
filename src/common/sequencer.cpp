#include "sequencer.h"

#include <algorithm>
#include <ranges>

using namespace hexx::common;

bool Sequencer::advance()
{
    std::erase_if(
        callbacks,
        [this](auto &callback_info)
        {
            if (tick < callback_info.start_tick)
            {
                return false;
            }

            return callback_info.callback(tick - callback_info.start_tick);
        });

    tick++;

    return callbacks.empty();
}