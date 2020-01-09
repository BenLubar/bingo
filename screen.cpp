#include "generator.h"

#include "modules/Screen.h"

enum class BingoField
{
    WinCondition,
    FreeSpace,
    num_fields,
};

class BingoScreen : public dfhack_viewscreen
{
public:
    BingoScreen(BingoScreenPage page) :
        page(page),
        sel_x(2),
        sel_y(2),
        sel_field(BingoField(0))
    {
    }

    bool isBoardPage() const
    {
        return page == BingoScreenPage::Default ||
            page == BingoScreenPage::Win ||
            page == BingoScreenPage::Loss;
    }

    void onShow() override
    {
        if (page == BingoScreenPage::Default && active_card)
        {
            // unopened file stream doesn't allow any output.
            std::ofstream dummy_fout;
            color_ostream_wrapper dummy_out(dummy_fout);
            switch (active_card->check(dummy_out))
            {
                case BingoState::NONE:
                    break;
                case BingoState::SUCCEEDED:
                    page = BingoScreenPage::Win;
                    break;
                case BingoState::FAILED:
                    page = BingoScreenPage::Loss;
                    break;
            }
        }
    }

    void feed(interface_key_set *keys) override
    {
        dfhack_viewscreen::feed(keys);

        if (keys->erase(interface_key::LEAVESCREEN))
        {
            Screen::dismiss(this);
            return;
        }

        if (isBoardPage() && active_card)
        {
            if (keys->erase(interface_key::CURSOR_LEFT))
            {
                sel_x--;
                if (sel_x < 0)
                {
                    sel_x = 4;
                }
            }
            if (keys->erase(interface_key::CURSOR_RIGHT))
            {
                sel_x++;
                if (sel_x >= 5)
                {
                    sel_x = 0;
                }
            }
            if (keys->erase(interface_key::CURSOR_UP))
            {
                sel_y--;
                if (sel_y < -1)
                {
                    sel_y = 4;
                }
            }
            if (keys->erase(interface_key::CURSOR_DOWN))
            {
                sel_y++;
                if (sel_y >= 5)
                {
                    sel_y = -1;
                }
            }
        }
        if (isBoardPage() && !active_card)
        {
            if (keys->erase(interface_key::SETUPGAME_NEW))
            {
                show_bingo_screen(BingoScreenPage::Generator);
                return;
            }
        }

        if (page == BingoScreenPage::Generator)
        {
            if (keys->erase(interface_key::MENU_CONFIRM))
            {
                should_update = true;
                active_card = generator.generate_card();
                force_win_check = true;
                Screen::dismiss(this);
                return;
            }

            if (keys->erase(interface_key::SETUPGAME_CUSTOMIZE_UNIT))
            {
                show_bingo_screen(BingoScreenPage::Configure);
                return;
            }
        }

        if (page == BingoScreenPage::Configure)
        {
            if (keys->erase(interface_key::CURSOR_DOWN))
            {
                int new_field = int(sel_field) + 1;
                if (new_field > int(BingoField::num_fields))
                {
                    new_field = 0;
                }
                sel_field = BingoField(new_field);
            }
            if (keys->erase(interface_key::CURSOR_UP))
            {
                int new_field = int(sel_field) - 1;
                if (new_field < 0)
                {
                    new_field = int(BingoField::num_fields) - 1;
                }
                sel_field = BingoField(new_field);
            }

            if (keys->erase(interface_key::CURSOR_LEFT))
            {
                inc_field(-1);
            }
            if (keys->erase(interface_key::CURSOR_RIGHT))
            {
                inc_field(1);
            }
        }
    }

    void logic() override
    {
        dfhack_viewscreen::logic();
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

        if (isBoardPage())
        {
            if (active_card)
            {
                render_card(*active_card);
            }
            else
            {
                Painter greeting(rect2d(df::coord2d(2, 2), dim - df::coord2d(3, 3)));
                greeting.string("There isn't an active bingo card. Press ");
                greeting.key(interface_key::SETUPGAME_NEW);
                greeting.string(" to generate one.");
            }
        }

        if (page == BingoScreenPage::Generator)
        {
            Painter instructions(rect2d(df::coord2d(2, 2), dim - df::coord2d(3, 3)));

            instructions.string("[");
            instructions.key(interface_key::MENU_CONFIRM);
            instructions.string("] Generate a bingo card with the current settings");
            instructions.newline();

            instructions.string("[");
            instructions.key(interface_key::SETUPGAME_CUSTOMIZE_UNIT);
            instructions.string("] Customize bingo card generator");
            instructions.newline();

            instructions.string("[");
            instructions.key(interface_key::LEAVESCREEN);
            instructions.string("] Leave this screen");
            instructions.newline();
        }

        if (page == BingoScreenPage::Configure)
        {
            Painter form(rect2d(df::coord2d(2, 2), dim - df::coord2d(3, 3)));
            Pen label(0, COLOR_WHITE, COLOR_BLACK);
            Pen selected(0, COLOR_BLACK, COLOR_GREY);
            Pen unselected(0, COLOR_GREY, COLOR_BLACK);

            form.string("Win Condition: ", label);
            form.string(generator.win_condition_candidates.at(generator.win_condition).summarize(), sel_field == BingoField::WinCondition ? selected : form.cur_pen);
            form.newline();

            form.string("Free Space: ", label);
            if (sel_field == BingoField::FreeSpace)
            {
                form.string("no", generator.free_space ? unselected : selected);
                form.advance(1);
                if (generator.free_space == 25)
                {
                    form.string("VERY YES", selected);
                }
                else
                {
                    form.string("yes", generator.free_space ? selected : unselected);
                }
            }
            else
            {
                if (generator.free_space == 25)
                {
                    form.string("VERY YES");
                }
                else if (generator.free_space)
                {
                    form.string("yes");
                }
                else
                {
                    form.string("no");
                }
            }
            form.newline();
        }
    }

    void render_card(const BingoCard & card)
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

        if (sel_y >= 0)
        {
            Pen selected('\xDB', COLOR_YELLOW);
            fillRect(selected, x0 + sel_x * dx, y0 + sel_y * dy, x0 + sel_x * dx + dx, y0 + sel_y * dy);
            fillRect(selected, x0 + sel_x * dx, y0 + sel_y * dy, x0 + sel_x * dx, y0 + sel_y * dy + dy);
            fillRect(selected, x0 + sel_x * dx, y0 + sel_y * dy + dy, x0 + sel_x * dx + dx, y0 + sel_y * dy + dy);
            fillRect(selected, x0 + sel_x * dx + dx, y0 + sel_y * dy, x0 + sel_x * dx + dx, y0 + sel_y * dy + dy);
        }

        Pen potential(0, COLOR_WHITE, COLOR_BLACK);
        Pen succeeded(0, COLOR_WHITE, COLOR_GREEN);
        Pen failed(0, COLOR_BLACK, COLOR_RED);

        for (int y = 0; y < 5; y++)
        {
            for (int x = 0; x < 5; x++)
            {
                auto & square = card.squares[y][x];
                Pen pen = square.state == BingoState::NONE ? potential :
                    square.state == BingoState::SUCCEEDED ? succeeded :
                    failed;

                auto desc = square.summarize();
                int y2 = 1;
                if (int(desc.length()) < (dx - 1) * (dy - 1))
                {
                    y2 += ((dx - 1) * (dy - 1) - desc.length()) / 2 / (dx - 1);
                }
                fillRect(pen, x0 + 1 + x * dx, y0 + 1 + y * dy, x0 - 1 + (x + 1) * dx, y0 - 1 + (y + 1) * dy);
                for (; y2 < dy; y2++)
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
        if (sel_y < 0)
        {
            goal = Pen(0, COLOR_BLACK, COLOR_GREY);
            fillRect(goal, x0, 1, x1, 1);
        }
        auto goalText = card.summarize();
        if (int(goalText.length()) > x1 - x0)
        {
            goalText.erase(x1 - x0);
        }
        paintString(goal, x0 + (x1 - x0 - goalText.length()) / 2, 1, goalText);

        int width = dim.x - x1 - 4;
        Pen text(0, COLOR_GREY, COLOR_BLACK);
        std::string summary;
        std::string description;
        if (sel_y >= 0)
        {
            summary = card.squares[sel_y][sel_x].summarize();
            description = card.squares[sel_y][sel_x].describe();
        }
        else
        {
            summary = card.summarize();
            description = card.describe();
        }
        if (int(summary.length()) > width)
        {
            summary.erase(width);
        }
        paintString(text, x1 + 2, 2, summary);
        for (int y = 4; !description.empty() && y < dim.y - 2; y++)
        {
            size_t len;
            if (int(description.length()) < width)
            {
                len = description.length();
            }
            else if ((len = description.rfind(' ', width)) == std::string::npos)
            {
                len = width;
            }
            else
            {
                len++;
            }

            size_t newline = description.find('\n');
            if (newline != std::string::npos && newline < len)
            {
                len = newline + 1;
                description[newline] = ' ';
            }

            paintString(text, x1 + 2, y, description.substr(0, len));
            description.erase(0, len);
        }

        Painter keys(rect2d(df::coord2d(2, dim.y - 2), df::coord2d(x1, dim.y - 2)));
        keys.string("move cursor:");
        keys.key(interface_key::CURSOR_LEFT);
        keys.key(interface_key::CURSOR_RIGHT);
        keys.key(interface_key::CURSOR_UP);
        keys.key(interface_key::CURSOR_DOWN);
        keys.string(" back:");
        keys.key(interface_key::LEAVESCREEN);
    }

    void inc_field(int amount)
    {
        switch (sel_field)
        {
            case BingoField::WinCondition:
                generator.win_condition += amount;
                break;
            case BingoField::FreeSpace:
                if (amount < 0)
                {
                    generator.free_space = 0;
                }
                else
                {
                    generator.free_space += amount;
                }
                break;
            case BingoField::num_fields:
                // should never happen
                break;
        }

        generator.check_bounds();
    }

    std::string getFocusString() override
    {
        switch (page)
        {
            case BingoScreenPage::Default:
                return "bingo/view";
            case BingoScreenPage::Win:
                return "bingo/view/win";
            case BingoScreenPage::Loss:
                return "bingo/view/loss";
            case BingoScreenPage::Generator:
                return "bingo/generator";
            case BingoScreenPage::Configure:
                return "bingo/generator/configure";
        }

        return "bingo/unknown";
    }

private:
    BingoScreenPage page;
    int sel_x, sel_y;
    BingoField sel_field;
};

DFhackDataExport extern Plugin *plugin_self;
bool show_bingo_screen(BingoScreenPage page)
{
    return Screen::show(std::unique_ptr<df::viewscreen>(new BingoScreen(page)), plugin_self);
}
