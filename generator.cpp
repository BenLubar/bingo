#include "generator.h"
#include "card.h"
#include "square.h"

#include <ctime>
#include <random>

#include "df/building_type.h"
#include "df/trap_type.h"

BingoGenerator::BingoGenerator() :
    win_condition(0),
    free_space(1),
    seed(0),
    win_condition_candidates(),
    objective_candidates()
{
    reset_candidates();
}

std::unique_ptr<BingoCard> BingoGenerator::generate_card() const
{
    auto card = std::unique_ptr<BingoCard>(new BingoCard());

    *card = win_condition_candidates[win_condition];

    std::vector<BingoSquare> squares = objective_candidates;
    std::shuffle(squares.begin(), squares.end(), std::mt19937(seed ? seed : int(std::time(nullptr))));

    // TODO: remove squares that violate generation requirements

    while (squares.size() < 25)
    {
        squares.push_back(BingoSquare());
    }

    int i = 0;
    for (auto & row : card->squares)
    {
        for (auto & square : row)
        {
            square = squares.at(i);
            i++;
        }
    }

    if (free_space == 25)
    {
        for (auto & row : card->squares)
        {
            for (auto & square : row)
            {
                square = BingoSquare();
                square.objective = BingoObjective::FREE_SPACE;
            }
        }
    }
    else if (free_space)
    {
        card->squares[2][2] = BingoSquare();
        card->squares[2][2].objective = BingoObjective::FREE_SPACE;
    }

    return card;
}

void BingoGenerator::reset_candidates()
{
    win_condition_candidates.clear();

    BingoCard card;

    card.win_condition = BingoWinCondition::FIVE_IN_A_ROW;
    win_condition_candidates.push_back(card);

    card.win_condition = BingoWinCondition::BLACKOUT;
    win_condition_candidates.push_back(card);

    objective_candidates.clear();

    BingoSquare square;

    square.objective = BingoObjective::TIME_LIMIT;
    square.data.clear();
    square.data["data1"] = 2;
    square.data1 = 2;
    square.data2 = 0;
    objective_candidates.push_back(square);

    square.objective = BingoObjective::POPULATION;
    square.data.clear();
    square.data["data1"] = 150;
    square.data1 = 150;
    square.data2 = 0;
    objective_candidates.push_back(square);

    square.objective = BingoObjective::MASS_DANCE;
    square.data.clear();
    square.data["data1"] = 20;
    square.data1 = 20;
    square.data2 = 0;
    objective_candidates.push_back(square);

    square.objective = BingoObjective::MOUNTAINHOME;
    square.data.clear();
    square.data1 = 0;
    square.data2 = 0;
    objective_candidates.push_back(square);

    square.objective = BingoObjective::REACH_HFS;
    square.data.clear();
    square.data1 = 0;
    square.data2 = 0;
    objective_candidates.push_back(square);

    square.objective = BingoObjective::AVOID_BUILDING;
    square.data.clear();
    square.data["data1"] = int(building_type::Trap);
    square.data["data2"] = int(trap_type::Lever) + 1;
    square.data1 = int(building_type::Trap);
    square.data2 = int(trap_type::Lever) + 1;
    objective_candidates.push_back(square);
}

void BingoGenerator::check_bounds()
{
    int max_win_condition = int(win_condition_candidates.size()) - 1;
    if (win_condition < 0)
    {
        win_condition = max_win_condition;
    }
    if (win_condition > max_win_condition)
    {
        win_condition = 0;
    }
    if (free_space < 0)
    {
        free_space = 0;
    }
    if (free_space > 25)
    {
        free_space = 26;
    }
}

BingoGenerator generator;
