#pragma once

#include <vector>
#include <cstdint>
#include <string>

namespace hexx::common
{
    /**
     * @brief Read a file into a vector of bytes
     *
     * @param path path to the file
     * @return std::vector<uint8_t> output buffer
     * @throws std::runtime_error if the file could not be opened
     */
    std::vector<uint8_t> read_file(std::string const &path);

    /**
     * @brief Write a vector of bytes to a file
     *
     * @param path path to the file
     * @param data input buffer
     * @throws std::runtime_error if the file could not be opened
     */
    void write_file(std::string const &path, std::vector<uint8_t> const &data);
}