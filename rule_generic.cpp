#include "rule.h"

template<>
bool check_rule<BingoRule::FREE_SPACE>(color_ostream &, BingoSquare & square)
{
    if (square.state == BingoState::SUCCEEDED)
    {
        return false;
    }

    square.state = BingoState::SUCCEEDED;
    return true;
}

template<>
std::string summarize_rule<BingoRule::FREE_SPACE>(const BingoSquare &)
{
    return "FREE SPACE";
}

template<>
std::string describe_rule<BingoRule::FREE_SPACE>(const BingoSquare &)
{
    return "This square has no requirement. It is a free space.";
}
