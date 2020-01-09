#include "bingo.h"

class BingoGenerator
{
public:
    BingoGenerator();

    std::unique_ptr<BingoCard> generate_card() const;
    void reset_candidates();
    void check_bounds();

    int win_condition;
    int free_space;
    int seed;

    std::vector<BingoCard> win_condition_candidates;
    std::vector<BingoSquare> objective_candidates;
};

extern BingoGenerator generator;
