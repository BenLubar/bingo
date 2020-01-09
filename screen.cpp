#include "bingo.h"

#include "modules/Screen.h"

class BingoScreen : public dfhack_viewscreen
{
public:
    BingoScreen(BingoScreenPage page) :
        page(page)
    {
    }

    void feed(interface_key_set *keys) override
    {
        dfhack_viewscreen::feed(keys);

        // TODO
    }

    void logic() override
    {
        dfhack_viewscreen::logic();

        // TODO
    }

    void render() override
    {
        using namespace DFHack::Screen;

        dfhack_viewscreen::render();

        auto dim = getWindowSize();

        clear();
        drawBorder("BINGO");
        if (page == BingoScreenPage::Win)
        {
            Pen text(0, 0, COLOR_LIGHTGREEN);
            paintString(text, (dim.x - 5) / 2, 0, "BINGO");
        }
        else if (page == BingoScreenPage::Loss)
        {
            Pen text(0, 0, COLOR_LIGHTRED);
            paintString(text, (dim.x - 5) / 2, 0, "OH NO");
        }

        // TODO
    }

    std::string getFocusString() override
    {
        switch (page)
        {
            case BingoScreenPage::Default:
                return "bingo";
            case BingoScreenPage::Win:
                return "bingo/win";
            case BingoScreenPage::Loss:
                return "bingo/loss";
        }

        return "bingo/unknown";
    }

private:
    BingoScreenPage page;
};

bool show_bingo_screen(Plugin *plugin, BingoScreenPage page)
{
    return Screen::show(std::unique_ptr<df::viewscreen>(new BingoScreen(page)), plugin);
}
