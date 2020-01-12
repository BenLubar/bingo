local _ENV = mkmodule('plugins.bingo')

--[[

Events:

* generateBingoCandidates
  Listen for this event in order to construct bingo objectives and win conditions for a new card.

Functions:

* addObjectiveCandidate(name, data)
  Should only be called during the handling of generateBingoCandidates.
  name must have been registered with registerObjectiveType before calling this function.
  data is arbitrary serializable data.
* addWinConditionCandidate(name, data)
  Should only be called during the handling of generateBingoCandidates.
  name must have been registered with registerWinConditionType before calling this function.
  data is arbitrary serializable data.
* registerObjectiveType(name, check, summary, description)
  name must be a unique string.
  check must be a check_square function.
  summary and description can be a string or a describe_square function.
* registerWinConditionType(name, check, summary, description)
  name must be a unique string.
  check must be a check_card function.
  summary and description can be a string or a describe_card function.

Types:

* class BingoCard
  readonly string win_condition;
  readonly BingoState squares[5][5];
  table data;
  readonly table meta;
* class BingoSquare
  readonly string objective;
  readonly BingoState state;
  table data;
* enum BingoState
  0: NONE
  1: SUCCEEDED
  2: FAILED
* function check_card(readonly BingoCard) -> BingoState
* function check_square(BingoSquare) -> BingoState
* function describe_card(readonly BingoCard) -> string
* function describe_square(readonly BingoSquare) -> string

]]

local json = require('json')

BingoState = {
    [0] = "NONE",
    [1] = "SUCCEEDED",
    [2] = "FAILED",
    NONE = 0,
    SUCCEEDED = 1,
    FAILED = 2
}

--[[

string-indexed table of tables:
- function check(obj) -> BingoState or nil
- function summarize(obj) -> string
- function describe(obj) -> string

]]
_registeredWinHandlers = _registeredWinHandlers or {}
_registeredObjHandlers = _registeredObjHandlers or {}
--[[

integer-indexed table of tables:
- either:
  - string win_condition
  - BingoState squares[5][5]
  - table data (read-write)
  - table meta
  or:
  - string objective
  - BingoState state
  - table data (read-write)
  or:
  - nil (empty slot)

]]
_liveObjects = _liveObjects or {}

local function roTable(table, name, allowData = false)
    local view = {}
    setmetatable(view, {
        __index = function(self, k)
            if table[k] == nil then
                error(name.." has no field named "..k)
            elseif type(table[k]) ~= "table" then
                return table[k]
            elseif allowData and k == "data" then
                return table[k]
            else
                return roTable(table[k], name.."."..k)
            end
        end,
        __newindex = function()
            error(name.." is read-only")
        end
    })
    return view
end

function _checkWin(idx, state)
    local obj = _liveObjects[idx]
    local impl = _registeredWinHandlers[obj.win_condition]

    obj.squares[0][0] = state[1]
    obj.squares[0][1] = state[2]
    obj.squares[0][2] = state[3]
    obj.squares[0][3] = state[4]
    obj.squares[0][4] = state[5]

    obj.squares[1][0] = state[6]
    obj.squares[1][1] = state[7]
    obj.squares[1][2] = state[8]
    obj.squares[1][3] = state[9]
    obj.squares[1][4] = state[10]

    obj.squares[2][0] = state[11]
    obj.squares[2][1] = state[12]
    obj.squares[2][2] = state[13]
    obj.squares[2][3] = state[14]
    obj.squares[2][4] = state[15]

    obj.squares[3][0] = state[16]
    obj.squares[3][1] = state[17]
    obj.squares[3][2] = state[18]
    obj.squares[3][3] = state[19]
    obj.squares[3][4] = state[20]

    obj.squares[4][0] = state[21]
    obj.squares[4][1] = state[22]
    obj.squares[4][2] = state[23]
    obj.squares[4][3] = state[24]
    obj.squares[4][4] = state[25]

    return impl.check(roTable(obj, "BingoCard")) or BingoState.NONE
end

function _checkObj(idx, state)
    local obj = _liveObjects[idx]
    local impl = _registeredWinHandlers[obj.objective]

    obj.state = state

    return impl.check(roTable(obj, "BingoSquare", true)) or obj.state
end

function _describe(idx, isObj, long)
    local obj = _liveObjects[idx]
    local impl = if isObj then
        _registeredObjHandlers[obj.objective]
    else 
        _registeredWinHandlers[obj.win_condition]
    end
    local view = if isObj then
        roTable(obj, "BingoSquare")
    else
        roTable(obj, "BingoCard")
    end

    return if long then
        impl.summarize(view)
    else
        impl.describe(view)
    end
end

local function newObject(obj)
    for i,v in lpairs(_liveObjects) do
        if v == nil then
            _liveObjects[i] = obj
            return i
        end
    end

    table.insert(_liveObjects, obj)
    return #_liveObjects
end

function _deleteObject(idx)
    _liveObjects[idx] = nil
end

function _createWinCondition(name, data=nil, meta=nil)
    local obj = {
        win_condition = name,
        squares = {
            [0]={[0]=0,0,0,0,0},
            {[0]=0,0,0,0,0},
            {[0]=0,0,0,0,0},
            {[0]=0,0,0,0,0},
            {[0]=0,0,0,0,0}
        },
        data = data or {},
        meta = meta or {}
    }

    return newObject(obj)
end

function _createObjective(name, state=nil, data=nil)
    local obj = {
        objective = name,
        state = state or BingoState.NONE,
        data = data or {}
    }

    return newObject(obj)
end

function _serializeData(idx)
    return json.encode(_liveObjects[idx].data)
end

local function registerType(typeMap, typeName, name, check, summary, description)
    if typeMap[name] then
        error("bingo win "..typeName.." already registered: "..name)
    end

    if type(check) ~= "function" then
        error("check must be a function (was "..type(check)..")")
    end

    if type(summary) == "string" then
        local s = summary
        summary = function()
            return s
        end
    end

    if type(summary) ~= "function" then
        error("summary must be a string or a function (was "..type(summary)..")")
    end

    if type(description) == "string" then
        local d = description
        description = function()
            return d
        end
    end

    if type(description) ~= "function" then
        error("description must be a string or a function (was "..type(description)..")")
    end

    typeMap[name] = {
        check = check,
        summarize = summary,
        describe = description
    }

    return name
end

function registerWinConditionType(name, check, summary, description)
    return registerType(_registeredWinHandlers, "win condition", name, check, summary, description)
end

function registerObjectiveType(name, check, summary, description)
    return registerType(_registeredObjHandlers, "objective", name, check, summary, description)
end

function addWinConditionCandidate(name, data = nil, meta = nil)
    if not _registeredWinHandlers[name] then
        error("unregistered bingo win condition type: "..name)
    end

    local index = _createWinCondition(name, data, meta)
    _addWinConditionCandidate(name, index)
end

function addObjectiveCandidate(name, data = nil)
    if not _registeredObjHandlers[name] then
        error("unregistered bingo objective type: "..name)
    end

    local index = _createObjective(name, nil, data)
    _addObjectiveCandidate(name, index)
end

return _ENV
