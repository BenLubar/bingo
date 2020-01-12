#include "card.h"
#include "square.h"
#include "generator.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "DataFuncs.h"
#include "LuaTools.h"
#pragma GCC diagnostic pop

static void _addWinConditionCandidate(const char *, int);
static void _addObjectiveCandidate(const char *, int);

DFHACK_PLUGIN_LUA_FUNCTIONS
{
    DFHACK_LUA_FUNCTION(_addWinConditionCandidate),
    DFHACK_LUA_FUNCTION(_addObjectiveCandidate),
    DFHACK_LUA_END
};

DEFINE_LUA_EVENT_NH_0(generateBingoCandidates);

DFHACK_PLUGIN_LUA_EVENTS
{
    DFHACK_LUA_EVENT(generateBingoCandidates),
    DFHACK_LUA_END
};

static int lua_popinteger(lua_State *s)
{
    int n = lua_tointeger(s, -1);
    lua_pop(s, 1);
    return n;
}

static std::string lua_popstring(lua_State *s)
{
    size_t len;
    const char *base = lua_tolstring(s, -1, &len);

    std::string str(base, len);
    lua_pop(s, 1);
    return str;
}

static void _addWinConditionCandidate(const char *name, int index)
{
    BingoCard card;
    card.win_condition = BingoWinCondition::LUA_SCRIPT;
    card.data["name"] = name;
    card.lua_ref.reset(new BingoLuaRef(index));

    generator.add_win_condition_candidate(card);
}

static void _addObjectiveCandidate(const char *name, int index)
{
    BingoSquare square;
    square.objective = BingoObjective::LUA_SCRIPT;
    square.data["name"] = name;
    square.lua_ref.reset(new BingoLuaRef(index));

    generator.add_objective_candidate(square);
}

void BingoGenerator::add_lua_candidates(color_ostream & out)
{
    generateBingoCandidates(out);
}

template<>
BingoState check_win_condition<BingoWinCondition::LUA_SCRIPT>(color_ostream & out, const BingoCard & card)
{
    using namespace Lua::Core;

    std::vector<int> state_vec;
    for (auto & row : card.squares)
    {
        for (auto & square : row)
        {
            state_vec.push_back(int(square.state));
        }
    }

    PushModulePublic(out, "plugins.bingo", "_checkWin");
    Push(card.lua_ref->index);
    PushVector(state_vec);
    if (!SafeCall(out, 2, 1))
    {
        return BingoState::FAILED;
    }

    return BingoState(lua_popinteger(State));
}

template<>
std::string summarize_win_condition<BingoWinCondition::LUA_SCRIPT>(const BingoCard & card)
{
    using namespace Lua::Core;

    PushModulePublic(*Lua::GetOutput(State), "plugins.bingo", "_describe");
    Push(card.lua_ref->index);
    Push(false);
    Push(false);
    if (!SafeCall(*Lua::GetOutput(State), 3, 1))
    {
        return "(lua error)";
    }

    return lua_popstring(State);
}

template<>
std::string describe_win_condition<BingoWinCondition::LUA_SCRIPT>(const BingoCard & card)
{
    using namespace Lua::Core;

    PushModulePublic(*Lua::GetOutput(State), "plugins.bingo", "_describe");
    Push(card.lua_ref->index);
    Push(false);
    Push(true);
    if (!SafeCall(*Lua::GetOutput(State), 3, 1))
    {
        return "(lua error)";
    }

    return lua_popstring(State);
}

template<>
bool check_objective<BingoObjective::LUA_SCRIPT>(color_ostream & out, BingoSquare & square)
{
    using namespace Lua::Core;

    PushModulePublic(out, "plugins.bingo", "_checkObj");
    Push(square.lua_ref->index);
    Push(int(square.state));
    if (!SafeCall(out, 2, 1))
    {
        return square.change_state(BingoState::FAILED);
    }

    return square.change_state(BingoState(lua_popinteger(State)));
}

template<>
std::string summarize_objective<BingoObjective::LUA_SCRIPT>(const BingoSquare & square)
{
    using namespace Lua::Core;

    PushModulePublic(*Lua::GetOutput(State), "plugins.bingo", "_describe");
    Push(square.lua_ref->index);
    Push(true);
    Push(false);
    if (!SafeCall(*Lua::GetOutput(State), 3, 1))
    {
        return "(lua error)";
    }

    return lua_popstring(State);
}

template<>
std::string describe_objective<BingoObjective::LUA_SCRIPT>(const BingoSquare & square)
{
    using namespace Lua::Core;

    PushModulePublic(*Lua::GetOutput(State), "plugins.bingo", "_describe");
    Push(square.lua_ref->index);
    Push(true);
    Push(true);
    if (!SafeCall(*Lua::GetOutput(State), 3, 1))
    {
        return "(lua error)";
    }

    return lua_popstring(State);
}

BingoLuaRef::~BingoLuaRef()
{
    if (!index)
    {
        return;
    }

    using namespace Lua::Core;

    PushModulePublic(*Lua::GetOutput(State), "plugins.bingo", "_deleteObject");
    Push(index);
    SafeCall(*Lua::GetOutput(State), 1, 0);

    index = 0;
}

void BingoCard::save_lua(Json::Value & value) const
{
    using namespace Lua::Core;

    PushModulePublic(*Lua::GetOutput(State), "plugins.bingo", "_serializeData");
    Push(lua_ref->index);
    if (SafeCall(*Lua::GetOutput(State), 1, 1))
    {
        value["d"]["lua"] = lua_popstring(State);
    }
}

bool BingoCard::load_lua(color_ostream & out)
{
    using namespace Lua::Core;

    PushModulePublic(out, "plugins.bingo", "_newWinCondition");

    Push(data["name"].asString());
    PushModulePublic(out, "json", "decode");
    Push(data["lua"].asString());
    if (!SafeCall(out, 1, 1))
    {
        lua_pop(State, 2); // remove function and name
        return false;
    }
    PushModulePublic(out, "json", "decode");
    std::ostringstream metastr;
    metastr << meta;
    Push(metastr.str());
    if (!SafeCall(out, 1, 1))
    {
        lua_pop(State, 3); // remove function, name, and data
        return false;
    }

    if (!SafeCall(out, 3, 1))
    {
        return false;
    }

    lua_ref.reset(new BingoLuaRef(lua_popinteger(State)));
    return true;
}

void BingoSquare::save_lua(Json::Value & value) const
{
    using namespace Lua::Core;

    PushModulePublic(*Lua::GetOutput(State), "plugins.bingo", "_serializeData");
    Push(lua_ref->index);
    if (SafeCall(*Lua::GetOutput(State), 1, 1))
    {
        value["d"]["lua"] = lua_popstring(State);
    }
}

bool BingoSquare::load_lua(color_ostream & out)
{
    using namespace Lua::Core;

    PushModulePublic(out, "plugins.bingo", "_newObjective");
    Push(data["name"].asString());
    Push(int(state));
    PushModulePublic(out, "json", "decode");
    Push(data["lua"].asString());
    if (!SafeCall(out, 1, 1))
    {
        lua_pop(State, 3); // remove function, name, and state
        return false;
    }

    if (!SafeCall(out, 3, 1))
    {
        return false;
    }

    lua_ref.reset(new BingoLuaRef(lua_popinteger(State)));
    return true;
}
