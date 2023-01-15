#pragma once

#include <vector>
#include <ranges>
#include <stdexcept>
#include <tuple>
#include <initializer_list>
#include <algorithm>

namespace hexx::common
{
    /**
     * @brief Converts cube coordinates to odd-q offset coordinates.
     *
     * @param q coordinate in Q axis
     * @param r coordinate in R axis
     * @param s coordinate in S axis
     * @return constexpr std::pair<int, int> coordinates in X and Y axis
     */
    constexpr std::pair<int, int> cube_to_oddq(int q, int r, int s)
    {
        int x = q;
        int y = r + (q - (q & 1)) / 2;

        return {x, y};
    }

    /**
     * @brief Converts odd-q offset coordinates to cube coordinates.
     *
     * @param x coordinate in X axis
     * @param y coordinate in Y axis
     * @return constexpr std::tuple<int, int, int> coordinates in Q, R and S axis
     */
    constexpr std::tuple<int, int, int> oddq_to_cube(int x, int y)
    {
        int q = x;
        int r = y - (x - (x & 1)) / 2;

        return {q, r, -q - r};
    }

    /**
     * @brief Represents a hexagonal tile grid. The grid representation is derived from a square grid, with every odd column shifted (also known as "odd-q" layout)
     *
     * @tparam T type of the tile
     */
    template <class T>
    class HexMap
    {
        int width{};
        int height{};
        std::vector<T> tiles{};

    public:
        class ConstFaceIterator;

        /**
         * @brief Iterator that iterates over faces of the hexagonal grid, left to right -> top to bottom.
         */
        class FaceIterator
        {
            friend class HexMap;
            HexMap<T> *map;
            int pos;

        public:
            using iterator_category = std::random_access_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T *;
            using reference = T &;

            FaceIterator() : map(nullptr), pos(0) {}
            FaceIterator(FaceIterator const &other) : map(other.map), pos(other.pos) {}
            FaceIterator(HexMap<T> *map, int pos) : map(map), pos(pos) {}

            int face_x() const
            {
                return pos % map->width;
            }

            int face_y() const
            {
                return pos / map->width;
            }

            /**
             * @brief Returns a FaceIterator using relative offset specified in cube coordinates:
             *   +s  -r
             *     __
             * -q /  \ +q
             *    \__/
             *   +r  -s
             *
             * @param q relative position in Q axis
             * @param r relative position in R axis
             * @param s relative position in S axis
             * @return FaceIterator iterator to specified tile on map or HexMap<T>::end() if out of bounds.
             */
            FaceIterator relative_cube(int q, int r, int s)
            {
                auto [aq, ar, as] = oddq_to_cube(face_x(), face_y());
                auto [x, y] = cube_to_oddq(q + aq, r + ar, s + as);

                // if specified position is out of bounds, return an iterator to ::end() for convenient handling.
                if (x < 0 || y < 0 || x >= map->width || y >= map->height)
                {
                    return map->end();
                }

                const auto new_pos = y * map->width + x;

                return FaceIterator(map, new_pos);
            }

            /**
             * @brief Returns the distance between two faces in cube coordinates.
             *
             * @param iter iterator to the other face
             * @return int distance between two faces
             */
            int distance_cube(FaceIterator const &iter) const
            {
                const auto [aq, ar, as] = oddq_to_cube(face_x(), face_y());
                const auto [bq, br, bs] = oddq_to_cube(iter.face_x(), iter.face_y());

                return std::max({std::abs(aq - bq), std::abs(ar - br), std::abs(as - bs)});
            }

            FaceIterator &operator++()
            {
                pos++;
                return *this;
            }

            FaceIterator operator++(int)
            {
                FaceIterator i = *this;
                ++(*this);
                return i;
            }

            FaceIterator &operator--()
            {
                pos--;
                return *this;
            }

            FaceIterator operator--(int)
            {
                FaceIterator i = *this;
                --(*this);
                return i;
            }

            T &operator*() const
            {
                return map->tiles[pos];
            }

            T *operator->() const
            {
                return map->tiles[pos];
            }

            friend bool operator==(FaceIterator const &self, FaceIterator const &other)
            {
                return self.pos == other.pos;
            }

            friend bool operator!=(FaceIterator const &self, FaceIterator const &other)
            {
                return self.pos != other.pos;
            }

            friend size_t operator-(FaceIterator const &self, FaceIterator const &other)
            {
                return self.pos - other.pos;
            }

            operator ConstFaceIterator() const
            {
                return ConstFaceIterator{map, pos};
            }
        };

        /**
         * @brief Iterator that iterates over faces of the hexagonal grid, left to right -> top to bottom.
         */
        class ConstFaceIterator
        {
            friend class HexMap;
            const HexMap<T> *map;
            int pos;

        public:
            using iterator_category = std::random_access_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = const T *;
            using reference = T const &;

            ConstFaceIterator() : map(nullptr), pos(0) {}
            ConstFaceIterator(ConstFaceIterator const &other) : map(other.map), pos(other.pos) {}
            ConstFaceIterator(const HexMap<T> *map, int pos) : map(map), pos(pos) {}

            int face_x() const
            {
                return pos % map->width;
            }

            int face_y() const
            {
                return pos / map->width;
            }

            /**
             * @brief Returns a FaceIterator using relative offset specified in cube coordinates:
             *   +s  -r
             *     __
             * -q /  \ +q
             *    \__/
             *   +r  -s
             *
             * @param q relative position in Q axis
             * @param r relative position in R axis
             * @param s relative position in S axis
             * @return FaceIterator iterator to specified tile on map or HexMap<T>::end() if out of bounds.
             */
            ConstFaceIterator relative_cube(int q, int r, int s) const
            {
                auto [aq, ar, as] = oddq_to_cube(face_x(), face_y());
                auto [x, y] = cube_to_oddq(q + aq, r + ar, s + as);

                // if specified position is out of bounds, return an iterator to ::end() for convenient handling.
                if (x < 0 || y < 0 || x >= map->width || y >= map->height)
                {
                    return map->cend();
                }

                const auto new_pos = y * map->width + x;

                return ConstFaceIterator(map, new_pos);
            }

            /**
             * @brief Returns the distance between two faces in cube coordinates.
             *
             * @param iter iterator to the other face
             * @return int distance between two faces
             */
            int distance_cube(ConstFaceIterator const &iter) const
            {
                const auto [aq, ar, as] = oddq_to_cube(face_x(), face_y());
                const auto [bq, br, bs] = oddq_to_cube(iter.face_x(), iter.face_y());

                return std::max({std::abs(aq - bq), std::abs(ar - br), std::abs(as - bs)});
            }

            ConstFaceIterator &operator++()
            {
                pos++;
                return *this;
            }

            ConstFaceIterator operator++(int)
            {
                ConstFaceIterator i = *this;
                ++(*this);
                return i;
            }

            ConstFaceIterator &operator--()
            {
                pos--;
                return *this;
            }

            ConstFaceIterator operator--(int)
            {
                ConstFaceIterator i = *this;
                --(*this);
                return i;
            }

            T const &operator*() const
            {
                return map->tiles[pos];
            }

            const T *operator->() const
            {
                return map->tiles[pos];
            }

            friend bool operator==(ConstFaceIterator const &self, ConstFaceIterator const &other)
            {
                return self.pos == other.pos;
            }

            friend bool operator!=(ConstFaceIterator const &self, ConstFaceIterator const &other)
            {
                return self.pos != other.pos;
            }

            friend size_t operator-(ConstFaceIterator const &self, ConstFaceIterator const &other)
            {
                return self.pos - other.pos;
            }
        };

        using iterator = FaceIterator;
        using const_iterator = ConstFaceIterator;

        HexMap() {}

        HexMap(HexMap const &other) : width(other.width),
                                      height(other.height),
                                      tiles(other.tiles) {}

        HexMap(HexMap &&other) : width(other.width),
                                 height(other.height),
                                 tiles(std::move(other.tiles)) {}

        HexMap(int width, int height, std::initializer_list<T> tiles) : width(width), height(height), tiles(tiles)
        {
            if (width * height != tiles.size())
            {
                throw std::invalid_argument("provided tiles buffer size doesn't match specified width and height");
            }
        }

        HexMap(int width, int height, std::vector<T> &&tiles) : width(width), height(height), tiles(std::move(tiles))
        {
            if (width * height != this->tiles.size())
            {
                throw std::invalid_argument("provided tiles buffer size doesn't match specified width and height");
            }
        }

        HexMap &operator=(HexMap const &other)
        {
            this->width = other.width;
            this->height = other.height;
            this->tiles = other.tiles;
            return *this;
        }

        HexMap &operator=(HexMap &&other)
        {
            this->width = other.width;
            this->height = other.height;
            this->tiles = std::move(other.tiles);
            return *this;
        }

        /**
         * @brief Returns an iterator to face (x,y) of the grid.
         *
         * @param x row of the grid
         * @param y column of the grid
         * @return FaceIterator
         * @throws out_of_range if x or y is out of bounds.
         */
        FaceIterator at(int x, int y)
        {
            if (x < 0 || y < 0 || x >= width || y >= height)
            {
                throw std::out_of_range("tile position out of bounds");
            }

            return FaceIterator{this, y * width + x};
        }

        /**
         * @brief Returns an iterator to face (x,y) of the grid.
         *
         * @param x row of the grid
         * @param y column of the grid
         * @return FaceIterator
         * @throws out_of_range if x or y is out of bounds.
         */
        ConstFaceIterator at(int x, int y) const
        {
            if (x < 0 || y < 0 || x >= width || y >= height)
            {
                throw std::out_of_range("tile position out of bounds");
            }

            return ConstFaceIterator{this, y * width + x};
        }

        int get_width() const
        {
            return width;
        }

        int get_height() const
        {
            return height;
        }

        FaceIterator begin()
        {
            return FaceIterator{this, 0};
        }

        FaceIterator end()
        {
            return FaceIterator{this, width * height};
        }

        ConstFaceIterator begin() const
        {
            return ConstFaceIterator{this, 0};
        }

        ConstFaceIterator end() const
        {
            return ConstFaceIterator{this, width * height};
        }

        ConstFaceIterator cbegin() const
        {
            return ConstFaceIterator{this, 0};
        }

        ConstFaceIterator cend() const
        {
            return ConstFaceIterator{this, width * height};
        }
    };
}