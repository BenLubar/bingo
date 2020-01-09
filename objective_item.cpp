#include "square.h"

template<>
bool check_objective<BingoObjective::ADAMANTINE_EQUIPMENT>(color_ostream &, BingoSquare &)
{
    // TODO
    return false;
}

template<>
std::string summarize_objective<BingoObjective::ADAMANTINE_EQUIPMENT>(const BingoSquare &)
{
    // TODO
    return "TODO";
}

template<>
std::string describe_objective<BingoObjective::ADAMANTINE_EQUIPMENT>(const BingoSquare &)
{
    // TODO
    return "TODO";
}

template<>
bool check_objective<BingoObjective::COPIES_OF_BOOK>(color_ostream &, BingoSquare &)
{
    // TODO
    return false;
}

template<>
std::string summarize_objective<BingoObjective::COPIES_OF_BOOK>(const BingoSquare &)
{
    // TODO
    return "TODO";
}

template<>
std::string describe_objective<BingoObjective::COPIES_OF_BOOK>(const BingoSquare &)
{
    // TODO
    return "TODO";
}
