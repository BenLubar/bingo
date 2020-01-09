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

DFhackDataExport extern std::vector<std::string>* plugin_globals;

// BingoObjective is a predefined objective for a bingo square.
enum class BingoObjective;

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
    BingoSquare();

    BingoObjective objective;
    BingoState state;

    // Data that modifies the meaning of the bingo square's objective.
    Json::Value data;

    // Cached versions of the data1 and data2 keys from the data object, for speed.
    // Set to 0 if the keys are non-numeric or absent. Should be treated as read-only.
    int data1, data2;

    bool check(color_ostream &);
    std::string summarize() const;
    std::string describe() const;

    bool load(color_ostream &, const Json::Value &);
    void save(Json::Value &) const;
};

// BingoWinCondition is a predefined bingo board win condition.
enum class BingoWinCondition;

// BingoCard is the board used for a Bingo game.
class BingoCard
{
public:
    BingoCard();

    std::array<std::array<BingoSquare, 5>, 5> squares;
    BingoWinCondition win_condition;

    // Data that modifies the meaning of the bingo board's win_condition.
    Json::Value data;

    // Data that describes the board itself.
    Json::Value meta;

    BingoState check(color_ostream &) const;
    std::string summarize() const;
    std::string describe() const;

    bool load(color_ostream &, const Json::Value &);
    void save(Json::Value &) const;
};

extern std::unique_ptr<BingoCard> active_card;

class weblegends_handler_v1;
bool bingo_weblegends_handler(weblegends_handler_v1 &, const std::string &);

enum class BingoScreenPage
{
    Default,
    Win,
    Loss,
};

bool show_bingo_screen(Plugin *, BingoScreenPage page = BingoScreenPage::Default);
