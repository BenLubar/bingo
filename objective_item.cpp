#include "square.h"
#include "util.h"

#include "modules/World.h"

#include "df/item_constructed.h"
#include "df/itemimprovement_pagesst.h"
#include "df/itemimprovement_writingst.h"
#include "df/items_other_id.h"
#include "df/world.h"

REQUIRE_GLOBAL(world);

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
bool check_objective<BingoObjective::COPIES_OF_BOOK>(color_ostream &, BingoSquare & square)
{
    if (!Core::getInstance().isMapLoaded() || !World::isFortressMode() || square.state == BingoState::SUCCEEDED)
    {
        return false;
    }

    std::map<int32_t, int> count;
    for (auto item : world->items.other[items_other_id::IN_PLAY])
    {
        auto constructed = virtual_cast<df::item_constructed>(item);
        if (!constructed)
        {
            continue;
        }

        for (auto imp : constructed->improvements)
        {
            std::vector<int32_t> *contents;
            if (auto pages = virtual_cast<df::itemimprovement_pagesst>(imp))
            {
                contents = &pages->contents;
            }
            else if (auto written = virtual_cast<df::itemimprovement_writingst>(imp))
            {
                contents = &written->contents;
            }
            else
            {
                continue;
            }

            for (auto c : *contents)
            {
                if (++count[c] >= square.data1)
                {
                    square.state = BingoState::SUCCEEDED;
                    return true;
                }
            }
        }
    }

    return false;
}

template<>
std::string summarize_objective<BingoObjective::COPIES_OF_BOOK>(const BingoSquare & square)
{
    std::ostringstream str;
    str << square.data1 << ordinal_suffix(square.data1) << " Edition";
    return str.str();
}

template<>
std::string describe_objective<BingoObjective::COPIES_OF_BOOK>(const BingoSquare & square)
{
    std::ostringstream str;
    str << "Have at least " << square.data1 << " copies of the same book in your fortress.";
    return str.str();
}
