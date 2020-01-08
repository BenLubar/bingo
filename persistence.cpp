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

bool BingoBoard::load(color_ostream &, const Json::Value &)
{
    // TODO
    return false;
}

void BingoSquare::save(Json::Value & value) const
{
    value["r"] = int(rule);
    value["s"] = int(state);
    value["d"] = data;
}

bool BingoSquare::load(color_ostream &, const Json::Value &)
{
    // TODO
    return false;
}
