#pragma once

#include <optional>
#include <string>
#include <type_traits>
#include <vector>
#include <iostream>

namespace hexx::cli
{
    /**
     * @brief Prompts the user to select an option from a list of options.
     *
     * @tparam T any iterable type that contains string-convertible values
     * @param options list of options
     * @param prompt prompt to display to the user
     * @return std::optional<typename T::value_type> selected option, or std::nullopt if the user entered an invalid option
     */
    template <class T>
    std::optional<typename T::value_type> prompt_option(T const &options, std::string const &prompt)
    {
        static_assert(std::is_arithmetic_v<typename T::value_type> || std::is_same_v<typename T::value_type, std::string>,
                      "options must contain string-convertible values");

        printf("%s: ", prompt.c_str());

        std::string result;
        std::cin >> result;

        for (auto itr = options.begin(); itr != options.end(); ++itr)
        {
            if constexpr (std::is_same_v<typename T::value_type, std::string>)
            {
                if (result == *itr)
                {
                    return {*itr};
                }
            }
            else if constexpr (std::is_same_v<typename T::value_type, char>)
            {
                if (result[0] == *itr)
                {
                    return {*itr};
                }
            }
            else if constexpr (std::is_arithmetic_v<typename T::value_type>)
            {
                if (std::to_string(*itr) == result)
                {
                    return {*itr};
                }
            }
        }

        return std::nullopt;
    }

    /**
     * @brief Prompts the user to select an option from a list of options, and loops until a valid option is selected.
     *
     * @tparam T any iterable type that contains string-convertible values
     * @param options list of options
     * @param prompt prompt to display to the user
     * @return T::value_type the selected option
     */
    template <class T>
    typename T::value_type prompt_option_loop(T const &options, std::string const &prompt)
    {
        while (true)
        {
            auto result = prompt_option(options, prompt);
            if (result.has_value())
            {
                return result.value();
            }
            else
            {
                printf("Invalid option selected!\n");
            }
        }
    }

    /**
     * @brief Repeats a string a specified number of times.
     *
     * @param str Input string
     * @param count Number of times to repeat the string
     * @return std::string Repeated string
     */
    inline std::string repeat_str(std::string const &str, size_t count)
    {
        std::string result;
        result.reserve(str.size() * count);

        for (auto i = 0; i < count; i++)
        {
            result.append(str);
        }

        return result;
    }

    /**
     * @brief Generates a vector containing a sequence of letters.
     *
     * @param count Number of letters to generate (must be less than 26)
     * @return std::vector<char> Vector containing the generated letters
     */
    std::vector<char> letter_vec(int count);

    /**
     * @brief Generates a "(x - x)" string containing the first and last elements of a vector.
     * This function assumes the vector contains at least one element.
     *
     * @param vec vector with char elements
     * @return std::string result string
     */
    inline std::string first_last(std::vector<char> const &vec)
    {
        using std::operator""s;
        return "("s + vec.front() + " - "s + vec.back() + ")"s;
    }

    /**
     * @brief Replaces all instances of a substring in a string with another substring.
     *
     * @param target string to modify
     * @param source substring to replace
     * @param replacement replacement substring
     */
    void replace_string(std::string &target, std::string const &source, std::string const &replacement);
}