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
//
// (defined in square.h)
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

    // Objective for this square on the card.
    BingoObjective objective;

    // Current state of this square. FAILED squares are failed forever.
    BingoState state;
    // Sets [state] to [new_state]. Returns true if they were not previously equal.
    bool change_state(BingoState new_state);

    // Data that modifies the meaning of the bingo square's [objective].
    Json::Value data;

    // Cached versions of the data1 and data2 keys from the [data] object, for speed.
    // Set to 0 if the keys are non-numeric or absent. Should be treated as read-only.
    int data1, data2;

    // Updates the [state] of this card. Returns true if [state] changed.
    //
    // This function may modify [data].
    bool check(color_ostream &);
    // Returns a short description of this square's objective.
    std::string summarize() const;
    // Returns a long description of this square's objective.
    std::string describe() const;

    bool load(color_ostream &, const Json::Value &);
    void save(Json::Value &) const;
};

// BingoWinCondition is a predefined bingo board win condition.
//
// (defined in card.h)
enum class BingoWinCondition;

// BingoCard is the card used for a game of Bingo.
class BingoCard
{
public:
    BingoCard();

    // Bingo squares, row-major order.
    std::array<std::array<BingoSquare, 5>, 5> squares;

    // Win condition for this card.
    BingoWinCondition win_condition;

    // Data that modifies the meaning of the bingo card's win_condition.
    Json::Value data;

    // Data that describes the card itself.
    Json::Value meta;

    // Checks whether the card's win condition has been fulfilled (SUCCEEDED),
    // the card's win condition is impossible to fulfil (FAILED).
    // Otherwise, returns NONE.
    BingoState check(color_ostream &) const;
    // Returns a short description of this card's win condition.
    std::string summarize() const;
    // Returns a long description of this card's win condition.
    std::string describe() const;

    bool load(color_ostream &, const Json::Value &);
    void save(Json::Value &) const;
};

// Protected by CoreSuspender
extern bool & should_update;
extern bool force_win_check;
extern std::unique_ptr<BingoCard> active_card;

enum class BingoPage
{
    Default,
    Win,
    Loss,
    Generator,
    GeneratorConfirm,
    Configure,
};

bool show_bingo_screen(BingoPage page = BingoPage::Default);

class weblegends_handler_v1;
bool bingo_weblegends_handler(weblegends_handler_v1 &, const std::string &);
