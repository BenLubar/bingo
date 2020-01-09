#include "card.h"
#include "square.h"

template<>
BingoState check_win_condition<BingoWinCondition::LUA_SCRIPT>(color_ostream &, const BingoCard &)
{
    // TODO
    return BingoState::NONE;
}

template<>
std::string summarize_win_condition<BingoWinCondition::LUA_SCRIPT>(const BingoCard &)
{
    // TODO
    return "(lua script)";
}

template<>
std::string describe_win_condition<BingoWinCondition::LUA_SCRIPT>(const BingoCard &)
{
    // TODO
    return "(lua script)";
}

template<>
bool check_objective<BingoObjective::LUA_SCRIPT>(color_ostream &, BingoSquare &)
{
    // TODO
    return false;
}

template<>
std::string summarize_objective<BingoObjective::LUA_SCRIPT>(const BingoSquare &)
{
    // TODO
    return "(lua script)";
}

template<>
std::string describe_objective<BingoObjective::LUA_SCRIPT>(const BingoSquare &)
{
    // TODO
    return "(lua script)";
}
