#include "square.h"

template<>
bool check_objective<BingoObjective::SURVIVE_GLACIER>(color_ostream &, BingoSquare &)
{
    // TODO
    return false;
}

template<>
std::string summarize_objective<BingoObjective::SURVIVE_GLACIER>(const BingoSquare &)
{
    // TODO
    return "TODO";
}

template<>
std::string describe_objective<BingoObjective::SURVIVE_GLACIER>(const BingoSquare &)
{
    // TODO
    return "TODO";
}

template<>
bool check_objective<BingoObjective::SURVIVE_WITHOUT_DRINK>(color_ostream &, BingoSquare &)
{
    // TODO
    return false;
}

template<>
std::string summarize_objective<BingoObjective::SURVIVE_WITHOUT_DRINK>(const BingoSquare &)
{
    // TODO
    return "TODO";
}

template<>
std::string describe_objective<BingoObjective::SURVIVE_WITHOUT_DRINK>(const BingoSquare &)
{
    // TODO
    return "TODO";
}
