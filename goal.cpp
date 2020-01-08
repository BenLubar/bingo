#include "goal.h"

#define BINGO_GOAL(x) template<> BingoState check_goal<BingoGoal::x>(color_ostream &, const BingoBoard &);
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
