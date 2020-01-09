#pragma once

#include "bingo.h"

#define BINGO_OBJECTIVES \
    BINGO_OBJECTIVE(UNKNOWN_OBJECTIVE) \
    BINGO_OBJECTIVE(LUA_SCRIPT) \
    BINGO_OBJECTIVE(FREE_SPACE)

enum class BingoObjective
{
#define BINGO_OBJECTIVE(x) x,
    BINGO_OBJECTIVES
#undef BINGO_OBJECTIVE
};

template<BingoObjective objective>
bool check_objective(color_ostream &, BingoSquare &);

template<BingoObjective objective>
std::string summarize_objective(const BingoSquare &);

template<BingoObjective objective>
std::string describe_objective(const BingoSquare &);
