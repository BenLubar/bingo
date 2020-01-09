#include "rule.h"

#define BINGO_RULE(x) \
    template<> bool check_rule<BingoRule::x>(color_ostream &, BingoSquare &); \
    template<> std::string summarize_rule<BingoRule::x>(const BingoSquare &); \
    template<> std::string describe_rule<BingoRule::x>(const BingoSquare &);
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

std::string BingoSquare::summarize() const
{
    switch (rule)
    {
#define BINGO_RULE(x) \
        case BingoRule::x: \
            return summarize_rule<BingoRule::x>(*this);
BINGO_RULES
#undef BINGO_RULE
    }

    return summarize_rule<BingoRule::UNKNOWN_RULE>(*this);
}

std::string BingoSquare::describe() const
{
    switch (rule)
    {
#define BINGO_RULE(x) \
        case BingoRule::x: \
            return describe_rule<BingoRule::x>(*this);
BINGO_RULES
#undef BINGO_RULE
    }

    return describe_rule<BingoRule::UNKNOWN_RULE>(*this);
}

template<>
std::string summarize_rule<BingoRule::UNKNOWN_RULE>(const BingoSquare &)
{
    return "ERROR";
}

template<>
std::string describe_rule<BingoRule::UNKNOWN_RULE>(const BingoSquare &)
{
    return "An error occurred. This square is invalid.";
}

BingoSquare::BingoSquare() :
    rule(BingoRule::UNKNOWN_RULE),
    state(BingoState::NONE),
    data(Json::objectValue),
    data1(0),
    data2(0)
{
}
