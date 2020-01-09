#pragma once

#include "bingo.h"

#define BINGO_RULES \
    BINGO_RULE(UNKNOWN_RULE) \
    BINGO_RULE(LUA_SCRIPT) \
    BINGO_RULE(FREE_SPACE)

enum class BingoRule
{
#define BINGO_RULE(x) x,
    BINGO_RULES
#undef BINGO_RULE
};

template<BingoRule rule>
bool check_rule(color_ostream &, BingoSquare &);

template<BingoRule rule>
std::string summarize_rule(const BingoSquare &);

template<BingoRule rule>
std::string describe_rule(const BingoSquare &);
