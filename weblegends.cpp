#include "bingo.h"
#include "weblegends-plugin.h"

bool bingo_weblegends_handler(weblegends_handler_v1 & ctx, const std::string & path)
{
    if (path == "/style.css")
    {
        ctx.headers()["Content-Type"] = "text/css";
        ctx.raw_out() << ".bingo-board {\n";
        ctx.raw_out() << "\ttable-layout: fixed;\n";
        ctx.raw_out() << "\tborder-collapse: collapse;\n";
        ctx.raw_out() << "}\n";
        ctx.raw_out() << ".bingo-board td {\n";
        ctx.raw_out() << "\twidth: 150px;\n";
        ctx.raw_out() << "\theight: 150px;\n";
        ctx.raw_out() << "\tborder: 1px solid #000;\n";
        ctx.raw_out() << "}\n";
        ctx.raw_out() << ".bingo-board .possible {\n";
        ctx.raw_out() << "\tbackground-color: #aaa;\n";
        ctx.raw_out() << "\tcolor: #000;\n";
        ctx.raw_out() << "}\n";
        ctx.raw_out() << ".bingo-board .succeeded {\n";
        ctx.raw_out() << "\tbackground-color: #6f6;\n";
        ctx.raw_out() << "\tcolor: #000;\n";
        ctx.raw_out() << "\tfont-weight: bold;\n";
        ctx.raw_out() << "}\n";
        ctx.raw_out() << ".bingo-board .failed {\n";
        ctx.raw_out() << "\tbackground-color: #800;\n";
        ctx.raw_out() << "\tcolor: #aaa;\n";
        ctx.raw_out() << "}\n";
        return true;
    }

    if (!path.empty())
    {
        return false;
    }

    ctx.headers()["Refresh"] = "5";

    ctx.raw_out() << "<link href=\"/bingo/style.css\" rel=\"stylesheet\">\n";
    if (!active_board)
    {
        ctx.raw_out() << "<i>no bingo currently in progress</i>\n";
        return true;
    }

    ctx.raw_out() << "<table class=\"bingo-board\">\n";
    ctx.raw_out() << "<caption>Goal: ";
    ctx.raw_out() << active_board->describe();
    ctx.raw_out() << "</caption>\n";
    for (auto & row : active_board->squares)
    {
        ctx.raw_out() << "<tr>\n";
        for (auto & square : row)
        {
            ctx.raw_out() << "<td class=\"";
            switch (square.state)
            {
                case BingoState::NONE:
                    ctx.raw_out() << "possible";
                    break;
                case BingoState::SUCCEEDED:
                    ctx.raw_out() << "succeeded";
                    break;
                case BingoState::FAILED:
                    ctx.raw_out() << "failed";
                    break;
            }
            ctx.raw_out() << "\">";
            ctx.raw_out() << square.describe();
            ctx.raw_out() << "</td>\n";
        }
        ctx.raw_out() << "</tr>\n";
    }
    ctx.raw_out() << "</table>\n";

    return true;
}
