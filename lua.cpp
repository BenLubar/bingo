#include "rule.h"
#include "goal.h"

template<>
BingoState check_goal<BingoGoal::LUA_SCRIPT>(color_ostream &, const BingoBoard &)
{
    // TODO
    return BingoState::NONE;
}

template<>
std::string summarize_goal<BingoGoal::LUA_SCRIPT>(const BingoBoard &)
{
    // TODO
    return "(lua script)";
}

template<>
std::string describe_goal<BingoGoal::LUA_SCRIPT>(const BingoBoard &)
{
    // TODO
    return "(lua script)";
}

template<>
bool check_rule<BingoRule::LUA_SCRIPT>(color_ostream &, BingoSquare &)
{
    // TODO
    return false;
}

template<>
std::string summarize_rule<BingoRule::LUA_SCRIPT>(const BingoSquare &)
{
    // TODO
    return "(lua script)";
}

template<>
std::string describe_rule<BingoRule::LUA_SCRIPT>(const BingoSquare &)
{
    // TODO
    return "(lua script)";
}
