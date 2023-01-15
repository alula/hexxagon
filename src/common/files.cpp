#include "files.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

std::vector<uint8_t> hexx::common::read_file(std::string const &path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + path);
    }

    auto size = file.tellg();
    std::vector<uint8_t> buffer(size);
    file.seekg(0);
    file.read(reinterpret_cast<char *>(buffer.data()), size);
    file.close();

    return buffer;
}

void hexx::common::write_file(std::string const &path, std::vector<uint8_t> const &data)
{
    std::ofstream file(path, std::ios::binary | std::ios::trunc);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + path);
    }

    file.write(reinterpret_cast<char const *>(data.data()), data.size());
    file.close();
}