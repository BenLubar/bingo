#pragma once

#include "bingo.h"

#define BINGO_WIN_CONDITIONS \
    BINGO_WIN_CONDITION(UNKNOWN_WIN_CONDITION) \
    BINGO_WIN_CONDITION(LUA_SCRIPT) \
    BINGO_WIN_CONDITION(FIVE_IN_A_ROW) \
    BINGO_WIN_CONDITION(BLACKOUT) \
    /* declare new win conditions here */

enum class BingoWinCondition
{
#define BINGO_WIN_CONDITION(x) x,
    BINGO_WIN_CONDITIONS
#undef BINGO_WIN_CONDITION
};

template<BingoWinCondition win_condition>
BingoState check_win_condition(color_ostream &, const BingoCard &);

template<BingoWinCondition win_condition>
std::string summarize_win_condition(const BingoCard &);

template<BingoWinCondition win_condition>
std::string describe_win_condition(const BingoCard &);
