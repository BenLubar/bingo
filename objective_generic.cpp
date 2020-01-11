#include "square.h"

template<>
bool check_objective<BingoObjective::FREE_SPACE>(color_ostream &, BingoSquare & square)
{
    return square.change_state(BingoState::SUCCEEDED);
}

template<>
std::string summarize_objective<BingoObjective::FREE_SPACE>(const BingoSquare &)
{
    return "FREE SPACE";
}

template<>
std::string describe_objective<BingoObjective::FREE_SPACE>(const BingoSquare &)
{
    return "This square has no requirement. It is a free space.";
}
