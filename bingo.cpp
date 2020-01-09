#include "bingo.h"
#include "weblegends-plugin.h"

DFHACK_PLUGIN("bingo");

command_result bingo_command(color_ostream &, std::vector<std::string> &);

DFhackCExport command_result plugin_init(color_ostream &, std::vector<PluginCommand> & commands)
{
#define DESCRIPTION "adds objectives and win conditions to Dwarf Fortress"

    commands.push_back(PluginCommand(
        "bingo",
        DESCRIPTION,
        bingo_command,
        false,
        "bingo: " DESCRIPTION "\n"
        "bingo show\n"
        "  displays the interactive bingo view\n"
    ));

    add_weblegends_handler("bingo", &bingo_weblegends_handler, "Bingo");

    return CR_OK;
}

DFhackCExport command_result plugin_shutdown(color_ostream &)
{
    remove_weblegends_handler("bingo");

    active_board = nullptr;

    return CR_OK;
}

DFhackCExport command_result plugin_onupdate(color_ostream & out)
{
    if (!active_board)
    {
        return CR_OK;
    }

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
                show_bingo_screen(plugin_self, BingoScreenPage::Win);
                break;
            case BingoState::FAILED:
                show_bingo_screen(plugin_self, BingoScreenPage::Loss);
                break;
        }
    }

    return CR_OK;
}

command_result bingo_command(color_ostream & out, std::vector<std::string> & parameters)
{
    if (parameters.size() == 1 && parameters.at(0) == "show")
    {
        if (show_bingo_screen(plugin_self))
        {
            return CR_OK;
        }

        out.printerr("failed to open bingo viewscreen!\n");

        return CR_FAILURE;
    }

    return CR_WRONG_USAGE;
}
