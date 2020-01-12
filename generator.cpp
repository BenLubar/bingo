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
}

void BingoGenerator::generate_and_activate_card(color_ostream & out)
{
    should_update = true;
    active_card = generate_card();
    force_win_check = true;
    reset_candidates(out);
}

std::unique_ptr<BingoCard> BingoGenerator::generate_card() const
{
    auto card = std::unique_ptr<BingoCard>(new BingoCard());

    *card = win_condition_candidates[win_condition];

    if (seed)
    {
        card->meta["seed"] = seed;
    }

    std::vector<BingoSquare> squares = objective_candidates;
    std::shuffle(squares.begin(), squares.end(), std::mt19937(seed ? seed : int(std::time(nullptr))));

    // TODO: remove squares that violate generation requirements

    squares.resize(25);

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

void BingoGenerator::reset_candidates(color_ostream & out)
{
    win_condition_candidates.clear();

    add_win_condition_candidate(BingoWinCondition::FIVE_IN_A_ROW);
    add_win_condition_candidate(BingoWinCondition::BLACKOUT);

    objective_candidates.clear();

    add_objective_candidate(BingoObjective::TIME_LIMIT, 2, 0);
    add_objective_candidate(BingoObjective::POPULATION, 150);
    add_objective_candidate(BingoObjective::MASS_DANCE, 20);
    add_objective_candidate(BingoObjective::MOUNTAINHOME);
    add_objective_candidate(BingoObjective::REACH_HFS);
    add_objective_candidate(BingoObjective::AVOID_BUILDING, int(building_type::Trap), int(trap_type::Lever) + 1);
    add_objective_candidate(BingoObjective::AVOID_BUILDING, int(building_type::Bed));

    add_lua_candidates(out);
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

void BingoGenerator::add_win_condition_candidate(BingoCard card)
{
    win_condition_candidates.push_back(card);
}

void BingoGenerator::add_win_condition_candidate(BingoWinCondition win_condition)
{
    BingoCard card;
    card.win_condition = win_condition;

    add_win_condition_candidate(card);
}

void BingoGenerator::add_objective_candidate(BingoSquare square)
{
    square.data1 = square.data["data1"].isIntegral() ? square.data["data1"].asInt() : 0;
    square.data2 = square.data["data2"].isIntegral() ? square.data["data2"].asInt() : 0;

    objective_candidates.push_back(square);
}

void BingoGenerator::add_objective_candidate(BingoObjective objective)
{
    BingoSquare square;
    square.objective = objective;

    add_objective_candidate(square);
}

void BingoGenerator::add_objective_candidate(BingoObjective objective, int data1)
{
    BingoSquare square;
    square.objective = objective;
    square.data["data1"] = data1;

    add_objective_candidate(square);
}

void BingoGenerator::add_objective_candidate(BingoObjective objective, int data1, int data2)
{
    BingoSquare square;
    square.objective = objective;
    square.data["data1"] = data1;
    square.data["data2"] = data2;

    add_objective_candidate(square);
}

BingoGenerator generator;
