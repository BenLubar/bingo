#include "bingo.h"

class BingoGenerator
{
public:
    BingoGenerator();

    std::unique_ptr<BingoCard> generate_card() const;
    void reset_candidates(color_ostream &);
    void check_bounds();

    int win_condition;
    int free_space;
    int seed;

    std::vector<BingoCard> win_condition_candidates;
    std::vector<BingoSquare> objective_candidates;
    void add_win_condition_candidate(BingoCard);
    void add_win_condition_candidate(BingoWinCondition);
    void add_objective_candidate(BingoSquare);
    void add_objective_candidate(BingoObjective);
    void add_objective_candidate(BingoObjective, int);
    void add_objective_candidate(BingoObjective, int, int);
};

extern BingoGenerator generator;
