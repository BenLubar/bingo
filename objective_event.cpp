#include "square.h"

#include "df/activity_entry.h"
#include "df/activity_event_performancest.h"
#include "df/entity_site_link.h"
#include "df/feature_init_underworld_from_layerst.h"
#include "df/historical_entity.h"
#include "df/ui.h"
#include "df/unit.h"
#include "df/world.h"

#include "modules/World.h"

REQUIRE_GLOBAL(ui);
REQUIRE_GLOBAL(world);

template<>
bool check_objective<BingoObjective::MOUNTAINHOME>(color_ostream &, BingoSquare & square)
{
    if (square.state == BingoState::SUCCEEDED || !Core::getInstance().isMapLoaded())
    {
        return false;
    }

    if (!World::isFortressMode())
    {
        // can only complete this objective in fortress mode.
        return false;
    }

    auto civ = df::historical_entity::find(ui->civ_id);
    if (!civ)
    {
        // should never happen
        return false;
    }

    for (auto & site_link : civ->site_links)
    {
        if (site_link->target != ui->site_id)
        {
            continue;
        }

        if (site_link->flags.bits.capital)
        {
            square.state = BingoState::SUCCEEDED;
            return true;
        }

        return false;
    }

    // should never happen
    return false;
}

template<>
std::string summarize_objective<BingoObjective::MOUNTAINHOME>(const BingoSquare &)
{
    return "Become the Mountain Home";
}

template<>
std::string describe_objective<BingoObjective::MOUNTAINHOME>(const BingoSquare &)
{
    return "Become the capital of your civilization through diplomacy.";
}

template<>
bool check_objective<BingoObjective::REACH_HFS>(color_ostream &, BingoSquare & square)
{
    if (square.state == BingoState::SUCCEEDED || !Core::getInstance().isMapLoaded())
    {
        return false;
    }

    for (auto & feature : world->features.map_features)
    {
        auto underworld = virtual_cast<df::feature_init_underworld_from_layerst>(feature);
        if (!underworld)
        {
            continue;
        }

        if (underworld->flags.is_set(feature_init_flags::Discovered))
        {
            square.state = BingoState::SUCCEEDED;
            return true;
        }
    }

    return false;
}

template<>
std::string summarize_objective<BingoObjective::REACH_HFS>(const BingoSquare &)
{
    return "Visit the Circus";
}

template<>
std::string describe_objective<BingoObjective::REACH_HFS>(const BingoSquare &)
{
    return "Go down to the circus on the other side of the cotton candy.\n\n"
        "There's lots of Hidden Fun Stuff at the circus. Beware of clowns.";
}

template<>
bool check_objective<BingoObjective::MASS_DANCE>(color_ostream &, BingoSquare & square)
{
    if (square.state == BingoState::SUCCEEDED || !Core::getInstance().isMapLoaded())
    {
        return false;
    }


    for (auto & activity : world->activities.all)
    {
        for (auto & event : activity->events)
        {
            auto performance = virtual_cast<df::activity_event_performancest>(event);
            if (!performance || performance->type != performance_event_type::DANCE)
            {
                continue;
            }

            if (int(performance->participants.units.size()) < square.data1)
            {
                continue;
            }

            int dancers = 0;
            for (auto & action : performance->participant_actions)
            {
                if (action && action->type == performance_participant_type::PERFORM_DANCE)
                {
                    dancers++;
                }
            }

            if (dancers < square.data1)
            {
                continue;
            }

            if (World::isAdventureMode())
            {
                bool found = false;
                auto player_character = world->units.active[0]->id;
                for (auto & unit : performance->participants.units)
                {
                    if (unit == player_character)
                    {
                        found = true;
                        break;
                    }
                }

                if (!found)
                {
                    continue;
                }
            }

            square.state = BingoState::SUCCEEDED;
            return true;
        }
    }

    return false;
}

template<>
std::string summarize_objective<BingoObjective::MASS_DANCE>(const BingoSquare & square)
{
    std::ostringstream str;
    str << "Takes " << square.data1 << " to Tango";
    return str.str();
}

template<>
std::string describe_objective<BingoObjective::MASS_DANCE>(const BingoSquare & square)
{
    std::ostringstream str;
    str << "Witness a dance with at least " << square.data1 << " participants.\n\n";
    str << "In adventure mode, you must be participating in the dance for it to count.";
    return str.str();
}
