#include "card.h"

static BingoState check_five_impl(const BingoCard & card, int x0, int y0, int dx, int dy)
{
    bool success = true;

    for (int i = 0; i < 5; i++)
    {
        switch (card.squares[y0 + dy * i][x0 + dx * i].state)
        {
            case BingoState::NONE:
                success = false;
                break;
            case BingoState::SUCCEEDED:
                break;
            case BingoState::FAILED:
                return BingoState::FAILED;
        }
    }

    return success ? BingoState::SUCCEEDED : BingoState::NONE;
}

static void check_five(color_ostream & out, const BingoCard & card, BingoState & aggregate, const char *name, int x0, int y0, int dx, int dy)
{
    if (aggregate == BingoState::SUCCEEDED)
    {
        return;
    }

    switch (check_five_impl(card, x0, y0, dx, dy))
    {
        case BingoState::NONE:
            // mark the card as being possible to win
            aggregate = BingoState::NONE;
            break;
        case BingoState::SUCCEEDED:
            out << "BINGO! Five in a row in the " << name << " containing:";
            for (int i = 0; i < 5; i++)
            {
                out.print(" %c%c", 'A' + x0 + dx * i, '1' + y0 + dy * i);
            }
            out << std::endl;
            aggregate = BingoState::SUCCEEDED;
            break;
        case BingoState::FAILED:
            break;
    }
}

template<>
BingoState check_win_condition<BingoWinCondition::FIVE_IN_A_ROW>(color_ostream & out, const BingoCard & card)
{
    BingoState aggregate = BingoState::FAILED;

    // check rows
    check_five(out, card, aggregate, "row", 0, 0, 1, 0);
    check_five(out, card, aggregate, "row", 0, 1, 1, 0);
    check_five(out, card, aggregate, "row", 0, 2, 1, 0);
    check_five(out, card, aggregate, "row", 0, 3, 1, 0);
    check_five(out, card, aggregate, "row", 0, 4, 1, 0);

    // check columns
    check_five(out, card, aggregate, "column", 0, 0, 0, 1);
    check_five(out, card, aggregate, "column", 1, 0, 0, 1);
    check_five(out, card, aggregate, "column", 2, 0, 0, 1);
    check_five(out, card, aggregate, "column", 3, 0, 0, 1);
    check_five(out, card, aggregate, "column", 4, 0, 0, 1);

    // check diagonals
    check_five(out, card, aggregate, "diagonal", 0, 0, 1, 1);
    check_five(out, card, aggregate, "diagonal", 4, 0, -1, 1);

    return aggregate;
}

template<>
std::string summarize_win_condition<BingoWinCondition::FIVE_IN_A_ROW>(const BingoCard &)
{
    return "Five-in-a-Row";
}

template<>
std::string describe_win_condition<BingoWinCondition::FIVE_IN_A_ROW>(const BingoCard &)
{
    return "Five squares in any row or column or either of the two diagonals are required to win.";
}

template<>
BingoState check_win_condition<BingoWinCondition::BLACKOUT>(color_ostream & out, const BingoCard & card)
{
    BingoState aggregate = BingoState::SUCCEEDED;

    for (auto & row : card.squares)
    {
        for (auto & square : row)
        {
            switch (square.state)
            {
                case BingoState::NONE:
                    aggregate = BingoState::NONE;
                    break;
                case BingoState::SUCCEEDED:
                    break;
                case BingoState::FAILED:
                    return BingoState::FAILED;
            }
        }
    }

    if (aggregate == BingoState::SUCCEEDED)
    {
        out << "BINGO! The entire card was completed!" << std::endl;
    }

    return aggregate;
}

template<>
std::string summarize_win_condition<BingoWinCondition::BLACKOUT>(const BingoCard &)
{
    return "Blackout";
}

template<>
std::string describe_win_condition<BingoWinCondition::BLACKOUT>(const BingoCard &)
{
    return "All 25 squares are required to win.";
}
