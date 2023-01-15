#pragma once

#include <vector>
#include <span>
#include <string>
#include <cstdint>

namespace hexx::common
{
    /**
     * @brief Utility class for serializing various types to a byte vector in a portable way.
     */
    class ByteWriter
    {
    public:
        std::vector<uint8_t> data;

        ByteWriter() = default;
        ByteWriter(std::vector<uint8_t> &&data) : data(std::move(data)) {}
        ByteWriter(std::vector<uint8_t> const &data) : data(data) {}
        ByteWriter(ByteWriter &&other) : data(std::move(other.data)) {}
        ByteWriter(ByteWriter const &other) : data(other.data) {}

        ByteWriter &operator=(ByteWriter &&other)
        {
            data = std::move(other.data);
            return *this;
        }

        ByteWriter &operator=(ByteWriter const &other)
        {
            data = other.data;
            return *this;
        }

        ByteWriter &write_uint8(uint8_t value)
        {
            data.push_back(value);
            return *this;
        }

        ByteWriter &write_uint16(uint16_t value)
        {
            data.push_back(value & 0xFF);
            data.push_back((value >> 8) & 0xFF);
            return *this;
        }

        ByteWriter &write_uint32(uint32_t value)
        {
            data.push_back(value & 0xFF);
            data.push_back((value >> 8) & 0xFF);
            data.push_back((value >> 16) & 0xFF);
            data.push_back((value >> 24) & 0xFF);
            return *this;
        }

        ByteWriter &write_uint64(uint64_t value)
        {
            data.push_back(value & 0xFF);
            data.push_back((value >> 8) & 0xFF);
            data.push_back((value >> 16) & 0xFF);
            data.push_back((value >> 24) & 0xFF);
            data.push_back((value >> 32) & 0xFF);
            data.push_back((value >> 40) & 0xFF);
            data.push_back((value >> 48) & 0xFF);
            data.push_back((value >> 56) & 0xFF);
            return *this;
        }

        ByteWriter &write_int8(int8_t value)
        {
            data.push_back(value);
            return *this;
        }

        ByteWriter &write_int16(int16_t value)
        {
            data.push_back(value & 0xFF);
            data.push_back((value >> 8) & 0xFF);
            return *this;
        }

        ByteWriter &write_int32(int32_t value)
        {
            data.push_back(value & 0xFF);
            data.push_back((value >> 8) & 0xFF);
            data.push_back((value >> 16) & 0xFF);
            data.push_back((value >> 24) & 0xFF);
            return *this;
        }

        ByteWriter &write_int64(int64_t value)
        {
            data.push_back(value & 0xFF);
            data.push_back((value >> 8) & 0xFF);
            data.push_back((value >> 16) & 0xFF);
            data.push_back((value >> 24) & 0xFF);
            data.push_back((value >> 32) & 0xFF);
            data.push_back((value >> 40) & 0xFF);
            data.push_back((value >> 48) & 0xFF);
            data.push_back((value >> 56) & 0xFF);
            return *this;
        }

        ByteWriter &write_string(std::string const &value)
        {
            write_uint32(value.size());
            data.insert(data.end(), value.begin(), value.end());
            return *this;
        }
    };

    /**
     * @brief Utility class for deserializing various types from a byte vector in a portable way.
     */
    class ByteReader
    {
    public:
        std::vector<uint8_t> data;
        size_t pos;

        ByteReader() : pos(0) {}
        ByteReader(std::vector<uint8_t> &&data) : data(std::move(data)), pos(0) {}
        ByteReader(std::vector<uint8_t> const &data) : data(data), pos(0) {}
        ByteReader(ByteReader &&other) : data(std::move(other.data)), pos(other.pos) {}
        ByteReader(ByteReader const &other) : data(other.data), pos(other.pos) {}

        ByteReader &operator=(ByteReader &&other)
        {
            data = std::move(other.data);
            pos = other.pos;
            return *this;
        }

        ByteReader &operator=(ByteReader const &other)
        {
            data = other.data;
            pos = other.pos;
            return *this;
        }

        uint8_t read_uint8()
        {
            return data.at(pos++);
        }

        uint16_t read_uint16()
        {
            uint16_t value = data.at(pos++);
            value |= data.at(pos++) << 8;
            return value;
        }

        uint32_t read_uint32()
        {
            uint32_t value = data.at(pos++);
            value |= data.at(pos++) << 8;
            value |= data.at(pos++) << 16;
            value |= data.at(pos++) << 24;
            return value;
        }

        uint64_t read_uint64()
        {
            uint64_t value = static_cast<uint64_t>(data.at(pos++));
            value |= static_cast<uint64_t>(data.at(pos++)) << 8;
            value |= static_cast<uint64_t>(data.at(pos++)) << 16;
            value |= static_cast<uint64_t>(data.at(pos++)) << 24;
            value |= static_cast<uint64_t>(data.at(pos++)) << 32;
            value |= static_cast<uint64_t>(data.at(pos++)) << 40;
            value |= static_cast<uint64_t>(data.at(pos++)) << 48;
            value |= static_cast<uint64_t>(data.at(pos++)) << 56;
            return value;
        }

        int8_t read_int8()
        {
            return data.at(pos++);
        }

        int16_t read_int16()
        {
            int16_t value = data.at(pos++);
            value |= data.at(pos++) << 8;
            return value;
        }

        int32_t read_int32()
        {
            int32_t value = data.at(pos++);
            value |= data.at(pos++) << 8;
            value |= data.at(pos++) << 16;
            value |= data.at(pos++) << 24;
            return value;
        }

        int64_t read_int64()
        {
            int64_t value = static_cast<int64_t>(data.at(pos++));
            value |= static_cast<int64_t>(data.at(pos++)) << 8;
            value |= static_cast<int64_t>(data.at(pos++)) << 16;
            value |= static_cast<int64_t>(data.at(pos++)) << 24;
            value |= static_cast<int64_t>(data.at(pos++)) << 32;
            value |= static_cast<int64_t>(data.at(pos++)) << 40;
            value |= static_cast<int64_t>(data.at(pos++)) << 48;
            value |= static_cast<int64_t>(data.at(pos++)) << 56;
            return value;
        }

        std::string read_string()
        {
            size_t size = read_uint32();
            std::string value(data.begin() + pos, data.begin() + pos + size);
            pos += size;
            return value;
        }
    };
};