#include "square.h"

template<>
bool check_objective<BingoObjective::ADOPTED_ANIMALS>(color_ostream &, BingoSquare &)
{
    // TODO
    return false;
}

template<>
std::string summarize_objective<BingoObjective::ADOPTED_ANIMALS>(const BingoSquare &)
{
    // TODO
    return "TODO";
}

template<>
std::string describe_objective<BingoObjective::ADOPTED_ANIMALS>(const BingoSquare &)
{
    // TODO
    return "TODO";
}

template<>
bool check_objective<BingoObjective::EVERYONE_IN_CAGES>(color_ostream &, BingoSquare &)
{
    // TODO
    return false;
}

template<>
std::string summarize_objective<BingoObjective::EVERYONE_IN_CAGES>(const BingoSquare &)
{
    // TODO
    return "TODO";
}

template<>
std::string describe_objective<BingoObjective::EVERYONE_IN_CAGES>(const BingoSquare &)
{
    // TODO
    return "TODO";
}

template<>
bool check_objective<BingoObjective::POPULATION>(color_ostream &, BingoSquare &)
{
    // TODO
    return false;
}

template<>
std::string summarize_objective<BingoObjective::POPULATION>(const BingoSquare &)
{
    // TODO
    return "TODO";
}

template<>
std::string describe_objective<BingoObjective::POPULATION>(const BingoSquare &)
{
    // TODO
    return "TODO";
}
