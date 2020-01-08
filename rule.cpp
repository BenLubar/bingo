#include "rule.h"

#define BINGO_RULE(x) template<> bool check_rule<BingoRule::x>(color_ostream &, BingoSquare &);
BINGO_RULES
#undef BINGO_RULE

bool BingoSquare::check(color_ostream & out)
{
    switch (rule)
    {
#define BINGO_RULE(x) \
        case BingoRule::x: \
            return check_rule<BingoRule::x>(out, *this);
BINGO_RULES
#undef BINGO_RULE
    }

    return check_rule<BingoRule::UNKNOWN_RULE>(out, *this);
}

template<>
bool check_rule<BingoRule::UNKNOWN_RULE>(color_ostream & out, BingoSquare & square)
{
    if (square.state == BingoState::FAILED)
    {
        return false;
    }

    out.printerr("unknown bingo square rule!\n");
    square.state = BingoState::FAILED;
    return true;
}
