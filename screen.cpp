#include "bingo.h"

#include "modules/Screen.h"

class BingoScreen : public dfhack_viewscreen
{
public:
    BingoScreen(BingoScreenPage page) :
        page(page),
        sel_x(2),
        sel_y(2)
    {
    }

    void feed(interface_key_set *keys) override
    {
        dfhack_viewscreen::feed(keys);

        if (keys->erase(interface_key::LEAVESCREEN))
        {
            Screen::dismiss(this);
            return;
        }

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
        drawBorder(" BINGO ");
        if (page == BingoScreenPage::Win)
        {
            Pen text(0, COLOR_LIGHTGREEN, COLOR_GREY);
            paintString(text, (dim.x - 7) / 2, 0, " BINGO ");
        }
        else if (page == BingoScreenPage::Loss)
        {
            Pen text(0, COLOR_LIGHTRED, COLOR_GREY);
            paintString(text, (dim.x - 7) / 2, 0, " OH NO ");
        }

        if (active_board)
        {
            render_board(*active_board);
        }

        // TODO
    }

    void render_board(const BingoBoard & board)
    {
        using namespace DFHack::Screen;

        auto dim = getWindowSize();

        Pen border('\xDB', COLOR_DARKGREY);
        int x0 = 2;
        int y0 = 2;
        int x1 = dim.x - 15;
        int y1 = dim.y - 3;
        if (x1 > y1)
        {
            x1 = y1;
        }
        int dx = (x1 - x0) / 5;
        int dy = (y1 - y0) / 5;
        x1 = x0 + dx * 5;
        y1 = y0 + dy * 5;

        for (int i = 0; i <= 5; i++)
        {
            fillRect(border, x0, y0 + i * dy, x1, y0 + i * dy);
            fillRect(border, x0 + i * dx, y0, x0 + i * dx, y1);
        }

        Pen selected('\xDB', COLOR_YELLOW);
        fillRect(selected, x0 + sel_x * dx, y0 + sel_y * dy, x0 + sel_x * dx + dx, y0 + sel_y * dy);
        fillRect(selected, x0 + sel_x * dx, y0 + sel_y * dy, x0 + sel_x * dx, y0 + sel_y * dy + dy);
        fillRect(selected, x0 + sel_x * dx, y0 + sel_y * dy + dy, x0 + sel_x * dx + dx, y0 + sel_y * dy + dy);
        fillRect(selected, x0 + sel_x * dx + dx, y0 + sel_y * dy, x0 + sel_x * dx + dx, y0 + sel_y * dy + dy);

        Pen potential(0, COLOR_WHITE, COLOR_BLACK);
        Pen succeeded(0, COLOR_WHITE, COLOR_GREEN);
        Pen failed(0, COLOR_BLACK, COLOR_RED);

        for (int y = 0; y < 5; y++)
        {
            for (int x = 0; x < 5; x++)
            {
                auto & square = board.squares[y][x];
                Pen pen = square.state == BingoState::NONE ? potential :
                    square.state == BingoState::SUCCEEDED ? succeeded :
                    failed;

                auto desc = square.describe();
                fillRect(pen, x0 + 1 + x * dx, y0 + 1 + y * dy, x0 - 1 + (x + 1) * dx, y0 - 1 + (y + 1) * dy);
                for (int y2 = 1; y2 < dy; y2++)
                {
                    auto trim = desc;
                    if (int(trim.length()) > dx - 1)
                    {
                        trim.erase(dx - 1);
                    }

                    paintString(pen, x0 + 1 + x * dx, y0 + y2 + y * dy, trim);

                    if (int(desc.length()) < dx - 1)
                    {
                        break;
                    }

                    desc.erase(0, dx - 1);
                }
            }
        }

        Pen goal(0, COLOR_WHITE, COLOR_BLACK);
        auto goalText = board.describe();
        if (int(goalText.length()) > x1 - x0)
        {
            goalText.erase(x1 - x0);
        }
        paintString(goal, x0 + (x1 - x0 - goalText.length()) / 2, 1, goalText);
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
    int sel_x, sel_y;
};

bool show_bingo_screen(Plugin *plugin, BingoScreenPage page)
{
    return Screen::show(std::unique_ptr<df::viewscreen>(new BingoScreen(page)), plugin);
}
