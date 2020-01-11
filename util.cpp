#include "util.h"

#include <sstream>

#include "modules/World.h"

#include "df/building.h"
#include "df/building_def_furnacest.h"
#include "df/building_def_workshopst.h"
#include "df/civzone_type.h"
#include "df/construction_type.h"
#include "df/furnace_type.h"
#include "df/shop_type.h"
#include "df/siegeengine_type.h"
#include "df/trap_type.h"
#include "df/workshop_type.h"
#include "df/world.h"

REQUIRE_GLOBAL(cur_year);
REQUIRE_GLOBAL(cur_year_tick);
REQUIRE_GLOBAL(cur_year_tick_advmode);
REQUIRE_GLOBAL(world);

void replace_all(std::string & s, const std::string & find, const std::string & replace)
{
    size_t find_size = find.length();
    size_t replace_size = replace.length();

    size_t i = s.find(find);
    while (i != std::string::npos)
    {
        s.replace(i, find_size, replace);
        i = s.find(find, i + replace_size);
    }
}

static const std::array<std::string, 12> month_names =
{
    "Granite",
    "Slate",
    "Felsite",
    "Hematite",
    "Malachite",
    "Galena",
    "Limestone",
    "Sandstone",
    "Timber",
    "Moonstone",
    "Opal",
    "Obsidian"
};

void date(std::ostream & out, int year, int tick)
{
    int day = tick / 1200;
    int month = day / 28;
    day = day % 28;

    out << (day + 1) << " " << month_names.at(month) << " " << year;
}

std::string add_spaces(std::string s)
{
#define CAPS "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    size_t i = s.find_first_of(CAPS, 1);
    while (i != std::string::npos)
    {
        s.insert(i, 1, ' ');
        i = s.find_first_of(CAPS, i + 1);
    }
    return s;
#undef CAPS
}

std::string get_building_name(const BingoSquare & square)
{
    auto type = df::building_type(square.data1);
    auto subtype = int16_t(square.data2 - 1);

    switch (type)
    {
        case building_type::Civzone:
            return add_spaces(ENUM_KEY_STR(civzone_type, df::civzone_type(type)));
        case building_type::Construction:
            return add_spaces(ENUM_KEY_STR(construction_type, df::construction_type(type)));
        case building_type::Furnace:
            if (subtype == furnace_type::Custom)
            {
                if (square.data["custom"].isString())
                {
                    auto code = square.data["custom"].asString();
                    for (auto & def : world->raws.buildings.furnaces)
                    {
                        if (def->code == code)
                        {
                            return def->name;
                        }
                    }
                }

                return "unknown custom furnace";
            }
            return ENUM_ATTR_STR(furnace_type, name, df::furnace_type(subtype));
        case building_type::Shop:
            return add_spaces(ENUM_KEY_STR(shop_type, df::shop_type(type)));
        case building_type::SiegeEngine:
            return add_spaces(ENUM_KEY_STR(siegeengine_type, df::siegeengine_type(type)));
        case building_type::Trap:
            return add_spaces(ENUM_KEY_STR(trap_type, df::trap_type(subtype)));
        case building_type::Workshop:
            if (subtype == workshop_type::Custom)
            {
                if (square.data["custom"].isString())
                {
                    auto code = square.data["custom"].asString();
                    for (auto & def : world->raws.buildings.workshops)
                    {
                        if (def->code == code)
                        {
                            return def->name;
                        }
                    }
                }

                return "unknown custom worshop";
            }
            return ENUM_ATTR_STR(workshop_type, name, df::workshop_type(subtype));
        default:
            return ENUM_ATTR_STR(building_type, name, type);
    }
}

bool matches_building(const BingoSquare & square, df::building *bld)
{
    auto type = df::building_type(square.data1);
    auto subtype = int16_t(square.data2 - 1);

    if (!bld || bld->getType() != type || bld->getSubtype() != subtype)
    {
        return false;
    }

    if (bld->getCustomType() != -1 && square.data["custom"].isString())
    {
        auto def = df::building_def::find(bld->getCustomType());
        if (!def || def->code != square.data["custom"].asString())
        {
            return false;
        }
    }

    return true;
}

void get_current_time(int & year, int & tick)
{
    year = *cur_year;
    tick = *cur_year_tick;
    if (World::isAdventureMode())
    {
        tick = *cur_year_tick_advmode / 72;
    }
}

bool has_time_passed(int years, int ticks, Json::Value & start_year, Json::Value & start_tick, int *end_year, int *end_tick)
{
    if (!start_year.isIntegral())
    {
        int now_year, now_tick;
        get_current_time(now_year, now_tick);
        start_year = now_year;
        start_tick = now_tick;
    }

    return has_time_passed(years, ticks, start_year.asInt(), start_tick.asInt(), end_year, end_tick);
}

bool has_time_passed(int years, int ticks, int start_year, int start_tick, int *end_year, int *end_tick)
{
    int now_year, now_tick;
    get_current_time(now_year, now_tick);

    int end_year_tmp, end_tick_tmp;
    if (!end_year)
    {
        end_year = &end_year_tmp;
    }
    if (!end_tick)
    {
        end_tick = &end_tick_tmp;
    }

    *end_year = start_year + years;
    *end_tick = start_tick + ticks;

    while (*end_tick >= 12 * 28 * 1200)
    {
        *end_tick = *end_tick - 12 * 28 * 1200;
        *end_year = *end_year + 1;
    }

    return now_year > *end_year || (now_year == *end_year && now_tick >= *end_tick);
}
