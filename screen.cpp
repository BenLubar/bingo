#include "bingo.h"

#include "modules/Screen.h"

class BingoScreen : public dfhack_viewscreen
{
public:
    BingoScreen(BingoScreenPage page) :
        page(page)
    {
    }

    void feed(std::set<df::interface_key> *keys)
    {
        dfhack_viewscreen::feed(keys);

        // TODO
    }

    void logic()
    {
        dfhack_viewscreen::logic();

        // TODO
    }

    void render()
    {
        dfhack_viewscreen::render();

        // TODO
    }

    std::string getFocusString() override
    {
        switch (page)
        {
            case BingoScreenPage::Default:
                return "bingo";
        }

        return "bingo/unknown";
    }

private:
    BingoScreenPage page;
};

void show_bingo_screen(Plugin *plugin, BingoScreenPage page)
{
    Screen::show(std::unique_ptr<df::viewscreen>(new BingoScreen(page)), plugin);
}
