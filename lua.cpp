#include "rule.h"
#include "goal.h"

template<>
BingoState check_goal<BingoGoal::LUA_SCRIPT>(color_ostream &, const BingoBoard &)
{
    // TODO
    return BingoState::NONE;
}

template<>
bool check_rule<BingoRule::LUA_SCRIPT>(color_ostream &, BingoSquare &)
{
    // TODO
    return false;
}

template<>
std::string describe_rule<BingoRule::LUA_SCRIPT>(const BingoSquare &)
{
    // TODO
    return "(lua script)";
}
