#pragma once

#include "bingo.h"

void replace_all(std::string & s, const std::string & find, const std::string & replace);
std::string add_spaces(std::string s);
void date(std::ostream & out, int year, int tick);
std::string get_building_name(const BingoSquare & square);
