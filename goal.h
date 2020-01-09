#pragma once

#include "bingo.h"

#define BINGO_GOALS \
    BINGO_GOAL(UNKNOWN_GOAL) \
    BINGO_GOAL(LUA_SCRIPT) \
    BINGO_GOAL(FIVE_IN_A_ROW) \
    BINGO_GOAL(BLACKOUT)

enum class BingoGoal
{
#define BINGO_GOAL(x) x,
    BINGO_GOALS
#undef BINGO_GOAL
};

template<BingoGoal goal>
BingoState check_goal(color_ostream &, const BingoBoard &);

template<BingoGoal goal>
std::string describe_goal(const BingoBoard &);
