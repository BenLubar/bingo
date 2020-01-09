#include "bingo.h"

#include "modules/Persistence.h"
#include "json/json.h"

std::unique_ptr<BingoBoard> active_board;

DFhackCExport command_result plugin_save_data(color_ostream &)
{
    auto file(Persistence::writeSaveData("bingo"));
    if (!active_board)
    {
        file << "null";
        return CR_OK;
    }

    Json::Value value(Json::objectValue);
    active_board->save(value);
    file << value;

    return CR_OK;
}

DFhackCExport command_result plugin_load_data(color_ostream & out)
{
    auto file(Persistence::readSaveData("bingo"));
    if (!file.good())
    {
        active_board = nullptr;
        return CR_OK;
    }

    Json::Value value(Json::nullValue);
    try
    {
        file >> value;
    }
    catch (std::exception & ex)
    {
        out.printerr("error loading bingo board: %s\n", ex.what());
        return CR_FAILURE;
    }

    if (value.isNull())
    {
        active_board = nullptr;
        return CR_OK;
    }

    std::unique_ptr<BingoBoard> board(new BingoBoard());
    if (!board->load(out, value))
    {
        return CR_FAILURE;
    }

    active_board = std::move(board);
    return CR_OK;
}

void BingoBoard::save(Json::Value & value) const
{
    Json::Value squaresValue(Json::arrayValue);
    for (auto & row : squares)
    {
        for (auto & square : row)
        {
            Json::Value squareValue(Json::objectValue);
            square.save(squareValue);
            squaresValue.append(squareValue);
        }
    }

    value["v"] = 0;
    value["s"] = squaresValue;
    value["g"] = int(goal);
    value["d"] = data;
    value["m"] = meta;
}

bool BingoBoard::load(color_ostream & out, const Json::Value & value)
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
        out.printerr("missing goal data in saved bingo file\n");
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

    goal = BingoGoal(value["g"].asInt());
    data = value["d"];
    meta = value["m"];

    return true;
}

void BingoSquare::save(Json::Value & value) const
{
    value["r"] = int(rule);
    value["s"] = int(state);
    value["d"] = data;
}

bool BingoSquare::load(color_ostream & out, const Json::Value & value)
{
    if (!value.isObject() || !value["r"].isIntegral() || !value["s"].isIntegral() || !value["d"].isObject())
    {
        out.printerr("invalid square in saved bingo file\n");
        return false;
    }

    rule = BingoRule(value["r"].asInt());
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

    return true;
}
