#include "utils.h"
#include <stdexcept>
#include <algorithm>

using namespace hexx::cli;

namespace hexx::cli
{
    std::vector<char> letter_vec(int count)
    {
        if (count > 26)
        {
            throw std::invalid_argument("count must be less than 26");
        }

        std::vector<char> vec(count);
        std::generate(vec.begin(), vec.end(), [n = 'a']() mutable
                      { return n++; });
        return vec;
    }

    void replace_string(std::string &target, std::string const &source, std::string const &replacement)
    {
        auto n = 0;
        while ((n = target.find(source, n)) != std::string::npos)
        {
            target.replace(n, source.size(), replacement);
            n += replacement.size();
        }
    }
}