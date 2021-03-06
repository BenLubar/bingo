#include "card.h"
#include "square.h"

#include "modules/Persistence.h"
#include "json/json.h"

std::unique_ptr<BingoCard> active_card;
bool force_win_check = false;

DFhackCExport command_result plugin_save_data(color_ostream &)
{
    auto file(Persistence::writeSaveData("bingo"));
    if (!active_card)
    {
        file << "null";
        return CR_OK;
    }

    Json::Value value(Json::objectValue);
    active_card->save(value);
    file << value;

    return CR_OK;
}

DFhackCExport command_result plugin_load_data(color_ostream & out)
{
    should_update = false;
    active_card = nullptr;

    auto file(Persistence::readSaveData("bingo"));
    if (!file.good())
    {
        return CR_OK;
    }

    Json::Value value(Json::nullValue);
    try
    {
        file >> value;
    }
    catch (std::exception & ex)
    {
        out.printerr("error loading bingo card: %s\n", ex.what());
        return CR_FAILURE;
    }

    if (value.isNull())
    {
        return CR_OK;
    }

    std::unique_ptr<BingoCard> card(new BingoCard());
    if (!card->load(out, value))
    {
        return CR_FAILURE;
    }

    should_update = true;
    active_card = std::move(card);
    force_win_check = true;
    return CR_OK;
}

void BingoCard::save(Json::Value & value) const
{
    Json::Value squares_value(Json::arrayValue);
    for (auto & row : squares)
    {
        for (auto & square : row)
        {
            Json::Value square_value(Json::objectValue);
            square.save(square_value);
            squares_value.append(square_value);
        }
    }

    value["v"] = 0;
    value["s"] = squares_value;
    value["g"] = int(win_condition);
    value["d"] = data;
    value["m"] = meta;
    if (lua_ref)
    {
        save_lua(value);
    }
}

bool BingoCard::load(color_ostream & out, const Json::Value & value)
{
    if (!value.isObject())
    {
        out.printerr("expected JSON object in bingo file\n");
        return false;
    }

    if (!value["v"].isIntegral())
    {
        out.printerr("missing version number in bingo file\n");
        return false;
    }

    int version = value["v"].asInt();
    switch (version)
    {
        case 0:
            // current version
            break;
        default:
            out.printerr("unexpected version number %d in bingo file\n", version);
            return false;
    }

    if (!value["s"].isArray() || value["s"].size() != 25)
    {
        out.printerr("missing squares in saved bingo file\n");
        return false;
    }

    if (!value["g"].isIntegral() || !value["d"].isObject() || !value["m"].isObject())
    {
        out.printerr("missing win condition data in saved bingo file\n");
        return false;
    }

    Json::ArrayIndex i = 0;
    for (auto & row : squares)
    {
        for (auto & square : row)
        {
            if (!square.load(out, value["s"][i]))
            {
                return false;
            }

            i++;
        }
    }

    win_condition = BingoWinCondition(value["g"].asInt());
    data = value["d"];
    meta = value["m"];

    if (win_condition == BingoWinCondition::LUA_SCRIPT)
    {
        return load_lua(out);
    }

    return true;
}

void BingoSquare::save(Json::Value & value) const
{
    value["r"] = int(objective);
    value["s"] = int(state);
    value["d"] = data;
    if (lua_ref)
    {
        save_lua(value);
    }
}

bool BingoSquare::load(color_ostream & out, const Json::Value & value)
{
    if (!value.isObject() || !value["r"].isIntegral() || !value["s"].isIntegral() || !value["d"].isObject())
    {
        out.printerr("invalid square in saved bingo file\n");
        return false;
    }

    objective = BingoObjective(value["r"].asInt());
    state = BingoState(value["s"].asInt());
    data = value["d"];

    data1 = 0;
    data2 = 0;

    if (data["data1"].isIntegral())
    {
        data1 = data["data1"].asInt();
    }
    if (data["data2"].isIntegral())
    {
        data2 = data["data2"].asInt();
    }

    if (objective == BingoObjective::LUA_SCRIPT)
    {
        return load_lua(out);
    }

    return true;
}
