#pragma once

#include "bingo.h"

namespace df
{
    struct building;
};

// replace all non-overlapping instances of [find] with [replace] in [s].
void replace_all(std::string & s, const std::string & find, const std::string & replace);
// inserts a space before every capital letter in [s].
std::string add_spaces(std::string s);
// writes the dwarf fortress date corresponding to the given [year] and [tick] to [out].
void date(std::ostream & out, int year, int tick);

// returns the name of a building with the following properties from [square].data:
// - data1: building_type
// - data2: subtype + 1 (data2 == 0 means any subtype, aka -1)
// - custom: custom type name (from raws)
std::string get_building_name(const BingoSquare & square);
// returns true if [bld] matches the properties of [square] as described above.
bool matches_building(const BingoSquare & square, df::building *bld);

// sets [year] and [tick] to the current fortress mode year and tick.
//
// if adventure mode is active, [tick] is scaled to match fortress mode timing.
// (72 adventure mode ticks per fortress mode tick).
void get_current_time(int & year, int & tick);

// a wrapper around the other overload of [has_time_passed] that automatically sets [start_year]
// and [start_tick] if they are not already defined.
bool has_time_passed(int years, int ticks, Json::Value & start_year, Json::Value & start_tick, int *end_year = nullptr, int *end_tick = nullptr);
// return true if the current time is more than [years] years and [ticks] ticks after [start_year]
// and [start_tick]. sets the values of [end_year] and [end_tick] if they are not nullptr.
bool has_time_passed(int years, int ticks, int start_year, int start_tick, int *end_year = nullptr, int *end_tick = nullptr);

std::string ordinal_suffix(int);
