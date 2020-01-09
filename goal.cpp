#include "goal.h"

#define BINGO_GOAL(x) \
    template<> BingoState check_goal<BingoGoal::x>(color_ostream &, const BingoBoard &); \
    template<> std::string summarize_goal<BingoGoal::x>(const BingoBoard &); \
    template<> std::string describe_goal<BingoGoal::x>(const BingoBoard &);
BINGO_GOALS
#undef BINGO_GOAL

BingoState BingoBoard::check(color_ostream & out) const
{
    switch (goal)
    {
#define BINGO_GOAL(x) \
        case BingoGoal::x: \
            return check_goal<BingoGoal::x>(out, *this);
BINGO_GOALS
#undef BINGO_GOAL
    }

    return check_goal<BingoGoal::UNKNOWN_GOAL>(out, *this);
}

template<>
BingoState check_goal<BingoGoal::UNKNOWN_GOAL>(color_ostream & out, const BingoBoard &)
{
    out.printerr("unknown bingo board goal!\n");
    return BingoState::FAILED;
}

std::string BingoBoard::summarize() const
{
    switch (goal)
    {
#define BINGO_GOAL(x) \
        case BingoGoal::x: \
            return summarize_goal<BingoGoal::x>(*this);
BINGO_GOALS
#undef BINGO_GOAL
    }

    return summarize_goal<BingoGoal::UNKNOWN_GOAL>(*this);
}

std::string BingoBoard::describe() const
{
    switch (goal)
    {
#define BINGO_GOAL(x) \
        case BingoGoal::x: \
            return describe_goal<BingoGoal::x>(*this);
BINGO_GOALS
#undef BINGO_GOAL
    }

    return describe_goal<BingoGoal::UNKNOWN_GOAL>(*this);
}

template<>
std::string summarize_goal<BingoGoal::UNKNOWN_GOAL>(const BingoBoard &)
{
    return "ERROR GOAL";
}

template<>
std::string describe_goal<BingoGoal::UNKNOWN_GOAL>(const BingoBoard &)
{
    return "An error occurred. The goal is invalid.";
}

BingoBoard::BingoBoard() :
    squares(),
    goal(BingoGoal::UNKNOWN_GOAL),
    data(Json::objectValue),
    meta(Json::objectValue)
{
}
