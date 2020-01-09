#include "util.h"

#include <sstream>

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
