#pragma once

#include <Console.h>
#include <Core.h>
#include <Export.h>
#include <DataDefs.h>
#include <PluginManager.h>

#include <array>
#include <memory> // for unique_ptr

#include "json/json.h"

using namespace DFHack;
using namespace df::enums;

// BingoRule is a predefined rule for a bingo square.
enum class BingoRule;

// BingoState is the current status of a bingo square or the entire board.
// Squares can move from NONE or SUCCEEDED to any value, but cannot leave FAILED state.
enum class BingoState
{
    NONE,
    SUCCEEDED,
    FAILED,
};

// BingoSquare is one square in the Bingo game.
class BingoSquare
{
public:
    BingoRule rule;
    BingoState state;

    // Data that modifies the meaning of the bingo square's rule.
    Json::Value data;

    // Cached versions of the data1 and data2 keys from the data object, for speed.
    // Set to 0 if the keys are non-numeric or absent. Should be treated as read-only.
    int data1, data2;

    bool check(color_ostream &);
    bool load(color_ostream &, const Json::Value &);
    void save(Json::Value &) const;
};

// BingoGoal is a predefined bingo board win condition.
enum class BingoGoal;

// BingoBoard is the board used for a Bingo game.
class BingoBoard
{
public:
    std::array<std::array<BingoSquare, 5>, 5> squares;
    BingoGoal goal;

    // Data that modifies the meaning of the bingo board's goal.
    Json::Value data;

    // Data that describes the board itself.
    Json::Value meta;

    BingoState check(color_ostream &) const;
    bool load(color_ostream &, const Json::Value &);
    void save(Json::Value &) const;
};

extern std::unique_ptr<BingoBoard> active_board;
