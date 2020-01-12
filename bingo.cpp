#include "bingo.h"
#include "generator.h"
#include "weblegends-plugin.h"

DFHACK_PLUGIN("bingo");
DFHACK_PLUGIN_IS_ENABLED(should_update);

command_result bingo_command(color_ostream &, std::vector<std::string> &);

DFhackCExport command_result plugin_init(color_ostream & out, std::vector<PluginCommand> & commands)
{
#define DESCRIPTION "adds objectives and win conditions to Dwarf Fortress"

    commands.push_back(PluginCommand(
        "bingo",
        DESCRIPTION,
        bingo_command,
        false,
        DESCRIPTION "\n"
        "bingo show\n"
        "  displays the interactive bingo card view\n"
        "bingo generate\n"
        "  generates a new bingo card without asking for confirmation\n"
        "bingo win-condition\n"
        "  displays the current win condition in the generator\n"
        "bingo win-condition list\n"
        "  lists possible win conditions\n"
        "bingo win-condition [number]\n"
        "  sets the win condition in the generator\n"
        "bingo free-space\n"
        "  displays the status of the free space\n"
        "bingo free-space off\n"
        "  turns off the free space\n"
        "bingo free-space on\n"
        "  turns on the free space\n"
        "bingo seed\n"
        "  displays the current seed\n"
        "bingo seed random\n"
        "  sets the seed to a random value per card\n"
        "bingo seed [number]\n"
        "  sets the seed to a specific value\n"
    ));

    add_weblegends_handler("bingo", &bingo_weblegends_handler, "Bingo");

    generator.reset_candidates(out);

    return CR_OK;
}

DFhackCExport command_result plugin_shutdown(color_ostream &)
{
    remove_weblegends_handler("bingo");

    should_update = false;
    active_card = nullptr;

    return CR_OK;
}

DFhackCExport command_result plugin_enable(color_ostream & out, bool enable)
{
    CoreSuspender suspend;

    if (enable)
    {
        out.printerr("Enabling the bingo plugin does nothing. See [help bingo] for usage information.\n");
        return CR_OK;
    }

    if (should_update)
    {
        out.printerr("Discarding the current bingo card to disable the plugin!\n");

        should_update = false;
        active_card = nullptr;
    }

    return CR_OK;
}

DFhackCExport command_result plugin_onupdate(color_ostream & out)
{
    if (!active_card)
    {
        should_update = false;
        return CR_OK;
    }

    bool changed = false;

    if (force_win_check)
    {
        changed = true;
        force_win_check = false;
    }

    for (auto & row : active_card->squares)
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
        switch (active_card->check(out))
        {
            case BingoState::NONE:
                // nothing to do
                break;
            case BingoState::SUCCEEDED:
                show_bingo_screen(BingoPage::Win);
                break;
            case BingoState::FAILED:
                show_bingo_screen(BingoPage::Loss);
                break;
        }
    }

    return CR_OK;
}

command_result bingo_command(color_ostream & out, std::vector<std::string> & parameters)
{
    CoreSuspender suspend;

    if (parameters.size() == 1 && parameters.at(0) == "show")
    {
        if (show_bingo_screen())
        {
            return CR_OK;
        }

        out.printerr("failed to open bingo viewscreen!\n");

        return CR_FAILURE;
    }

    if (parameters.size() == 1 && parameters.at(0) == "generate")
    {
        generator.generate_and_activate_card(out);
        return CR_OK;
    }

    if (parameters.size() == 1 && parameters.at(0) == "win-condition")
    {
        out << "The currently-selected win condition is " << generator.win_condition;
        out << ": " << generator.win_condition_candidates.at(generator.win_condition).summarize();
        out << std::endl;

        return CR_OK;
    }

    if (parameters.size() == 2 && parameters.at(0) == "win-condition")
    {
        if (parameters.at(1) == "list")
        {
            out << "Available win conditions are:" << std::endl;
            for (size_t i = 0; i < generator.win_condition_candidates.size(); i++)
            {
                out << i << ": " << generator.win_condition_candidates.at(i).summarize() << std::endl;
            }
            return CR_OK;
        }

        int index;
        try
        {
            index = std::stoi(parameters.at(1));
        }
        catch (std::exception &)
        {
            out.printerr("Invalid win condition number!\n");
            return CR_FAILURE;
        }

        if (index < 0 || index >= int(generator.win_condition_candidates.size()))
        {
            out.printerr("Invalid win condition number!\n");
            return CR_FAILURE;
        }

        generator.win_condition = index;
        generator.check_bounds();
        return CR_OK;
    }

    if (parameters.size() == 1 && parameters.at(0) == "free-space")
    {
        if (generator.free_space == 25)
        {
            out << "The free space is currently VERY ON." << std::endl;
        }
        else if (generator.free_space)
        {
            out << "The free space is currently on." << std::endl;
        }
        else
        {
            out << "The free space is currently off." << std::endl;
        }

        return CR_OK;
    }

    if (parameters.size() == 2 && parameters.at(0) == "free-space" && parameters.at(1) == "off")
    {
        generator.free_space = 0;
        generator.check_bounds();
        return CR_OK;
    }

    if (parameters.size() == 2 && parameters.at(0) == "free-space" && parameters.at(1) == "on")
    {
        generator.free_space++;
        generator.check_bounds();
        return CR_OK;
    }

    if (parameters.size() == 1 && parameters.at(0) == "seed")
    {
        if (generator.seed)
        {
            out << "The seed is currently: " << generator.seed << std::endl;
        }
        else
        {
            out << "The seed is currently random." << std::endl;
        }

        return CR_OK;
    }

    if (parameters.size() == 2 && parameters.at(0) == "seed")
    {
        if (parameters.at(1) == "random")
        {
            generator.seed = 0;
            generator.check_bounds();
            return CR_OK;
        }

        int seed;
        try
        {
            seed = std::stoi(parameters.at(1));
        }
        catch (std::exception &)
        {
            out.printerr("Invalid seed value!\n");
            return CR_FAILURE;
        }

        if (seed <= 0 || seed >= 1e9)
        {
            out.printerr("Invalid seed value!\n");
            return CR_FAILURE;
        }

        generator.seed = seed;
        generator.check_bounds();
        return CR_OK;
    }

    return CR_WRONG_USAGE;
}
