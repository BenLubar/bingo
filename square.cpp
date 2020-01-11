#include "square.h"

#define BINGO_OBJECTIVE(x) \
    template<> bool check_objective<BingoObjective::x>(color_ostream &, BingoSquare &); \
    template<> std::string summarize_objective<BingoObjective::x>(const BingoSquare &); \
    template<> std::string describe_objective<BingoObjective::x>(const BingoSquare &);
BINGO_OBJECTIVES
#undef BINGO_OBJECTIVE

bool BingoSquare::change_state(BingoState new_state)
{
    if (state == new_state)
    {
        return false;
    }

    state = new_state;
    return true;
}

bool BingoSquare::check(color_ostream & out)
{
    switch (objective)
    {
#define BINGO_OBJECTIVE(x) \
        case BingoObjective::x: \
            return check_objective<BingoObjective::x>(out, *this);
BINGO_OBJECTIVES
#undef BINGO_OBJECTIVE
    }

    return check_objective<BingoObjective::UNKNOWN_OBJECTIVE>(out, *this);
}

template<>
bool check_objective<BingoObjective::UNKNOWN_OBJECTIVE>(color_ostream & out, BingoSquare & square)
{
    if (square.state == BingoState::FAILED)
    {
        return false;
    }

    out.printerr("unknown bingo square objective!\n");
    square.state = BingoState::FAILED;
    return true;
}

std::string BingoSquare::summarize() const
{
    switch (objective)
    {
#define BINGO_OBJECTIVE(x) \
        case BingoObjective::x: \
            return summarize_objective<BingoObjective::x>(*this);
BINGO_OBJECTIVES
#undef BINGO_OBJECTIVE
    }

    return summarize_objective<BingoObjective::UNKNOWN_OBJECTIVE>(*this);
}

std::string BingoSquare::describe() const
{
    switch (objective)
    {
#define BINGO_OBJECTIVE(x) \
        case BingoObjective::x: \
            return describe_objective<BingoObjective::x>(*this);
BINGO_OBJECTIVES
#undef BINGO_OBJECTIVE
    }

    return describe_objective<BingoObjective::UNKNOWN_OBJECTIVE>(*this);
}

template<>
std::string summarize_objective<BingoObjective::UNKNOWN_OBJECTIVE>(const BingoSquare &)
{
    return "ERROR";
}

template<>
std::string describe_objective<BingoObjective::UNKNOWN_OBJECTIVE>(const BingoSquare &)
{
    return "An error occurred. This square is invalid.";
}

BingoSquare::BingoSquare() :
    objective(BingoObjective::UNKNOWN_OBJECTIVE),
    state(BingoState::NONE),
    data(Json::objectValue),
    data1(0),
    data2(0)
{
}
