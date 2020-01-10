#include "square.h"

#include "modules/Units.h"
#include "modules/World.h"

#include "df/world.h"

REQUIRE_GLOBAL(world);

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
bool check_objective<BingoObjective::POPULATION>(color_ostream &, BingoSquare & square)
{
    if (square.state == BingoState::SUCCEEDED || !Core::getInstance().isMapLoaded())
    {
        return false;
    }

    if (!World::isFortressMode())
    {
        return false;
    }

    int population = 0;
    for (auto & unit : world->units.active)
    {
        if (!Units::isSane(unit))
            continue;
        if (!Units::isCitizen(unit) && (!unit->flags2.bits.resident || !Units::isOwnGroup(unit)))
            continue;

        population++;
    }

    if (population >= square.data1)
    {
        square.state = BingoState::SUCCEEDED;
        return true;
    }

    return false;
}

template<>
std::string summarize_objective<BingoObjective::POPULATION>(const BingoSquare & square)
{
    std::ostringstream str;
    str << "Population: " << square.data1;
    return str.str();
}

template<>
std::string describe_objective<BingoObjective::POPULATION>(const BingoSquare & square)
{
    std::ostringstream str;
    str << "Reach a population of " << square.data1 << " citizens and permanent residents in your fortress.\n\n";
    str << "Visitors, animals, and the deceased don't count.";
    return str.str();
}
