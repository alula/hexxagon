#include "highscore_manager.h"
#include "files.h"
#include "byte_utils.h"

#include <ranges>

using namespace hexx::common;

// HighScoreManager();

// virtual ~HighScoreManager() = default;

// void add_score(int ruby, int pearl);

// void load_scores();

// void save_scores();

HighScoreManager::HighScoreManager()
{
    load_scores();
}

void HighScoreManager::add_score(int ruby, int pearl)
{
    ScoreInfo score;
    score.ruby = ruby;
    score.pearl = pearl;
    if (ruby > pearl)
    {
        score.winner = Winner::Ruby;
    }
    else if (ruby < pearl)
    {
        score.winner = Winner::Pearl;
    }
    else
    {
        score.winner = Winner::Draw;
    }

    // Determine top 5 scores
    // Prefer higher scores over draws
    scores.push_back(score);
    std::ranges::sort(scores, [](const ScoreInfo &a, const ScoreInfo &b) {
        if (a.winner == Winner::Draw && b.winner != Winner::Draw)
        {
            return false;
        }
        else if (a.winner != Winner::Draw && b.winner == Winner::Draw)
        {
            return true;
        }
        else if (a.winner == Winner::Draw && b.winner == Winner::Draw)
        {
            return a.ruby + a.pearl > b.ruby + b.pearl;
        }
        else
        {
            return a.ruby + a.pearl > b.ruby + b.pearl;
        }
    });

    save_scores();
}

constexpr static uint32_t MAGIC_NUMBER = 0x263065C0;

void HighScoreManager::load_scores()
{
    try
    {
        auto buffer = read_file("scores.dat");
        ByteReader reader{std::move(buffer)};

        if (reader.read_uint32() != MAGIC_NUMBER)
        {
            return;
        }

        auto version = reader.read_uint16();
        if (version != 1)
        {
            return;
        }

        auto score_count = reader.read_uint32();
        for (int i = 0; i < score_count; i++)
        {
            ScoreInfo score;
            score.winner = static_cast<Winner>(reader.read_uint8());
            score.ruby = reader.read_uint32();
            score.pearl = reader.read_uint32();
            scores.push_back(score);
        }
    }
    catch (std::exception &e)
    {
        // ignore
    }
}

void HighScoreManager::save_scores()
{
    ByteWriter writer;
    writer.write_uint32(MAGIC_NUMBER);
    writer.write_uint16(1);

    writer.write_uint32(scores.size());
    for (auto &score : scores)
    {
        writer.write_uint8(static_cast<uint32_t>(score.winner));
        writer.write_uint32(score.ruby);
        writer.write_uint32(score.pearl);
    }

    write_file("scores.dat", writer.data);
}
