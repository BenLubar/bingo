#include "square.h"
#include "util.h"

#include <sstream>

#include "modules/World.h"

#include "df/building.h"
#include "df/building_def_furnacest.h"
#include "df/building_def_workshopst.h"
#include "df/buildings_other_id.h"
#include "df/world.h"

REQUIRE_GLOBAL(world);

template<>
bool check_objective<BingoObjective::TIME_LIMIT>(color_ostream &, BingoSquare & square)
{
    if (!Core::getInstance().isMapLoaded())
    {
        // Don't do anything until embark happens.
        return false;
    }

    if (has_time_passed(square.data1, square.data2, square.data["start_year"], square.data["start_tick"]))
    {
        square.state = BingoState::FAILED;
        return true;
    }

    return square.change_state(BingoState::SUCCEEDED);
}

template<>
std::string summarize_objective<BingoObjective::TIME_LIMIT>(const BingoSquare & square)
{
    std::ostringstream str;

    if (square.data1 || !square.data2)
    {
        str << square.data1 << "-Year ";
    }
    if (square.data2)
    {
        int days = square.data2 / 1200;
        if (days > 28 || days <= 0)
        {
            str << (days / 28) << "-Month ";
        }
        if (days % 28)
        {
            str << (days % 28) << "-Day ";
        }
    }
    str << "Limit";

    return str.str();
}

template<>
std::string describe_objective<BingoObjective::TIME_LIMIT>(const BingoSquare & square)
{
    int days = square.data2 / 1200;
    int months = days / 28;
    days = days % 28;

    std::ostringstream str;

    str << "This objective will fail ";
    str << square.data1 << (square.data1 == 1 ? " year" : " years");
    if (months || days)
    {
        str << ", ";
        str << months << (months == 1 ? " month, and " : " months, and ");
        str << days << (days == 1 ? " day " : " days ");
    }
    else
    {
        str << " ";
    }
    str << "after the first embark.";

    str << "\n\n";

    if (!square.data.isMember("start_year"))
    {
        str << "No embarks have happened yet, so the timer has not yet started.";
    }
    else
    {
        int start_year = square.data["start_year"].asInt();
        int start_tick = square.data["start_tick"].asInt();

        int end_year, end_tick;
        bool failed = has_time_passed(square.data1, square.data2, start_year, start_tick, &end_year, &end_tick);

        str << "The first embark was on ";
        date(str, start_year, start_tick);
        str << ", so this objective ";
        str << (failed ? "failed " : "will fail ");
        str << "on ";
        date(str, end_year, end_tick);
        str << ".";
    }

    int now_year, now_tick;
    get_current_time(now_year, now_tick);

    str << "\n\nIt is currently ";
    date(str, now_year, now_tick);
    str << ".";

    return str.str();
}

template<>
bool check_objective<BingoObjective::AVOID_BUILDING>(color_ostream &, BingoSquare & square)
{
    if (!Core::getInstance().isMapLoaded())
    {
        // Don't do anything while the map isn't loaded.
        return false;
    }

    if (World::isAdventureMode())
    {
        // objective cannot be completed in adventure mode.
        return square.change_state(BingoState::NONE);
    }

    for (auto & bld : world->buildings.other[buildings_other_id::IN_PLAY])
    {
        if (matches_building(square, bld))
        {
            square.state = BingoState::FAILED;
            return true;
        }
    }

    return square.change_state(BingoState::SUCCEEDED);
}

template<>
std::string summarize_objective<BingoObjective::AVOID_BUILDING>(const BingoSquare & square)
{
    return get_building_name(square) + "-Free Fortress";
}

template<>
std::string describe_objective<BingoObjective::AVOID_BUILDING>(const BingoSquare & square)
{
    auto name = get_building_name(square);

    std::ostringstream str;
    str << "Complete other objectives without access to a " << name << "!\n\n";
    str << "Constructing even a single " << name << " will fail this objective.\n\n";
    str << "Note: For furniture, placing is what fails the objective; the corresponding item can still be built in a workshop. ";
    str << "This objective does not count as a win condition while playing adventure mode.";
    return str.str();
}
