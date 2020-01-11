#pragma once

#include "bingo.h"

// replace all non-overlapping instances of [find] with [replace] in [s].
void replace_all(std::string & s, const std::string & find, const std::string & replace);
// inserts a space before every capital letter in [s].
std::string add_spaces(std::string s);
// writes the dwarf fortress date corresponding to the given [year] and [tick] to [out].
void date(std::ostream & out, int year, int tick);
// returns the name of a building with the following properties from [square].data:
// - data1: building_type
// - data2: subtype + 1 (data2 == 0 means any subtype, aka -1)
// - code: custom type name (from raws)
std::string get_building_name(const BingoSquare & square);
