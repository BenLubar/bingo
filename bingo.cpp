#include "bingo.h"

DFHACK_PLUGIN("bingo");

DFhackCExport command_result plugin_init(color_ostream &, std::vector<PluginCommand> &)
{
    return CR_OK;
}

DFhackCExport command_result plugin_shutdown(color_ostream &)
{
    active_board = nullptr;

    return CR_OK;
}

DFhackCExport command_result plugin_onupdate(color_ostream & out)
{
    if (active_board)
    {
        bool changed = false;

        for (auto & row : active_board->squares)
        {
            for (auto & square : row)
            {
                if (square.state != BingoState::FAILED)
                {
                    changed = square.check(out) || changed;
                }
            }
        }

        if (changed)
        {
            switch (active_board->check(out))
            {
                case BingoState::NONE:
                    // nothing to do
                    break;
                case BingoState::SUCCEEDED:
                    // TODO: win notification
                    break;
                case BingoState::FAILED:
                    // TODO: loss notification
                    break;
            }
        }
    }

    return CR_OK;
}
