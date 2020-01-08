#include "bingo.h"

#include <modules/Persistence.h>

DFhackCExport command_result plugin_save_data(color_ostream &out)
{
    auto file(Persistence::writeSaveData("bingo"));

    // TODO

    return CR_OK;
}

DFhackCExport command_result plugin_load_data(color_ostream &out)
{
    auto file(Persistence::readSaveData("bingo"));

    // TODO

    return CR_OK;
}
