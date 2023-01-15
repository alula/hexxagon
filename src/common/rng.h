#pragma once

#include <random>

namespace hexx::common
{
    /**
     * @brief Picks a random element from a container
     *
     * @tparam T a container type
     * @param container the container to pick from
     * @return T::reference a reference to the picked element
     */
    template <class T>
    typename T::reference random_choice(T &container)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, container.size() - 1);
        return container[dis(gen)];
    }
}