#pragma once

#include "bingo.h"

#define BINGO_OBJECTIVES \
    BINGO_OBJECTIVE(UNKNOWN_OBJECTIVE) \
    BINGO_OBJECTIVE(LUA_SCRIPT) \
    BINGO_OBJECTIVE(FREE_SPACE) \
    BINGO_OBJECTIVE(TIME_LIMIT) \
    BINGO_OBJECTIVE(ADAMANTINE_EQUIPMENT) \
    BINGO_OBJECTIVE(ADOPTED_ANIMALS) \
    BINGO_OBJECTIVE(AVOID_BUILDING) \
    BINGO_OBJECTIVE(COPIES_OF_BOOK) \
    BINGO_OBJECTIVE(EVERYONE_IN_CAGES) \
    BINGO_OBJECTIVE(MASS_DANCE) \
    BINGO_OBJECTIVE(MOUNTAINHOME) \
    BINGO_OBJECTIVE(POPULATION) \
    BINGO_OBJECTIVE(REACH_HFS) \
    BINGO_OBJECTIVE(SURVIVE_LAKE) \
    BINGO_OBJECTIVE(SURVIVE_WITHOUT_DRINK) \
    /* declare new objectives here */

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
