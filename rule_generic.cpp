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
