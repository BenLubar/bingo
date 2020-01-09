#include "card.h"

#define BINGO_WIN_CONDITION(x) \
    template<> BingoState check_win_condition<BingoWinCondition::x>(color_ostream &, const BingoCard &); \
    template<> std::string summarize_win_condition<BingoWinCondition::x>(const BingoCard &); \
    template<> std::string describe_win_condition<BingoWinCondition::x>(const BingoCard &);
BINGO_WIN_CONDITIONS
#undef BINGO_WIN_CONDITION

BingoState BingoCard::check(color_ostream & out) const
{
    switch (win_condition)
    {
#define BINGO_WIN_CONDITION(x) \
        case BingoWinCondition::x: \
            return check_win_condition<BingoWinCondition::x>(out, *this);
BINGO_WIN_CONDITIONS
#undef BINGO_WIN_CONDITION
    }

    return check_win_condition<BingoWinCondition::UNKNOWN_WIN_CONDITION>(out, *this);
}

template<>
BingoState check_win_condition<BingoWinCondition::UNKNOWN_WIN_CONDITION>(color_ostream & out, const BingoCard &)
{
    out.printerr("unknown bingo card win condition!\n");
    return BingoState::FAILED;
}

std::string BingoCard::summarize() const
{
    switch (win_condition)
    {
#define BINGO_WIN_CONDITION(x) \
        case BingoWinCondition::x: \
            return summarize_win_condition<BingoWinCondition::x>(*this);
BINGO_WIN_CONDITIONS
#undef BINGO_WIN_CONDITION
    }

    return summarize_win_condition<BingoWinCondition::UNKNOWN_WIN_CONDITION>(*this);
}

std::string BingoCard::describe() const
{
    switch (win_condition)
    {
#define BINGO_WIN_CONDITION(x) \
        case BingoWinCondition::x: \
            return describe_win_condition<BingoWinCondition::x>(*this);
BINGO_WIN_CONDITIONS
#undef BINGO_WIN_CONDITION
    }

    return describe_win_condition<BingoWinCondition::UNKNOWN_WIN_CONDITION>(*this);
}

template<>
std::string summarize_win_condition<BingoWinCondition::UNKNOWN_WIN_CONDITION>(const BingoCard &)
{
    return "ERROR";
}

template<>
std::string describe_win_condition<BingoWinCondition::UNKNOWN_WIN_CONDITION>(const BingoCard &)
{
    return "An error occurred. The win condition is invalid.";
}

BingoCard::BingoCard() :
    squares(),
    win_condition(BingoWinCondition::UNKNOWN_WIN_CONDITION),
    data(Json::objectValue),
    meta(Json::objectValue)
{
}
