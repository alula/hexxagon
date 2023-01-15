#pragma once

#include "board.h"

namespace hexx::common
{
    /**
     * @brief Class that manages the high scores and loads/saves them to a file.
     */
    class HighScoreManager
    {
    public:
        enum class Winner
        {
            Ruby,
            Pearl,
            Draw
        };

        struct ScoreInfo
        {
            Winner winner;
            int ruby;
            int pearl;
        };

        std::vector<ScoreInfo> scores;

        HighScoreManager();

        virtual ~HighScoreManager() = default;

        /**
         * @brief Adds a score to the high score list and saves it to the file.
         *
         * @param ruby the score of the ruby player
         * @param pearl the score of the pearl player
         */
        void add_score(int ruby, int pearl);

        /**
         * @brief Loads the high scores from a file.
         */
        void load_scores();

        /**
         * @brief Saves the high scores to a file.
         */
        void save_scores();
    };
}