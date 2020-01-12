#include "bingo.h"
#include "util.h"
#include "weblegends-plugin.h"

std::string clean_string(std::string s, bool newlines)
{
    replace_all(s, "&", "&amp;");
    replace_all(s, "<", "&lt;");
    replace_all(s, ">", "&gt;");
    replace_all(s, "\"", "&quot;");
    if (newlines)
    {
        replace_all(s, "\n", "<br>");
        replace_all(s, "<br><br>", "<p>");
    }

    return s;
}

bool bingo_weblegends_handler(weblegends_handler_v1 & ctx, const std::string & path)
{
    if (path == "/style.css")
    {
        ctx.headers()["Content-Type"] = "text/css";
        ctx.raw_out() << "* {\n";
        ctx.raw_out() << "\tbox-sizing: border-box;\n";
        ctx.raw_out() << "}\n";
        ctx.raw_out() << ".bingo-card {\n";
        ctx.raw_out() << "\ttable-layout: fixed;\n";
        ctx.raw_out() << "\tmargin: 0 auto;\n";
        ctx.raw_out() << "\tborder-collapse: collapse;\n";
        ctx.raw_out() << "}\n";
        ctx.raw_out() << ".bingo-card td {\n";
        ctx.raw_out() << "\twidth: 175px;\n";
        ctx.raw_out() << "\theight: 175px;\n";
        ctx.raw_out() << "\tborder: 1px solid #000;\n";
        ctx.raw_out() << "\tpadding: 0;\n";
        ctx.raw_out() << "}\n";
        ctx.raw_out() << ".bingo-card td:not(:hover) p {\n";
        ctx.raw_out() << "\tmargin-top: 5px;\n";
        ctx.raw_out() << "\tborder-top: 1px solid;\n";
        ctx.raw_out() << "\tfont-size: 0;\n";
        ctx.raw_out() << "}\n";
        ctx.raw_out() << ".bingo-card td div {\n";
        ctx.raw_out() << "\tmax-height: 173px;\n";
        ctx.raw_out() << "\tpadding: 10px;\n";
        ctx.raw_out() << "\ttext-align: center;\n";
        ctx.raw_out() << "\toverflow-y: auto;\n";
        ctx.raw_out() << "}\n";
        ctx.raw_out() << ".bingo-card .possible {\n";
        ctx.raw_out() << "\tbackground-color: #eee;\n";
        ctx.raw_out() << "\tcolor: #000;\n";
        ctx.raw_out() << "}\n";
        ctx.raw_out() << ".bingo-card .succeeded {\n";
        ctx.raw_out() << "\tbackground-color: #6f6;\n";
        ctx.raw_out() << "\tcolor: #000;\n";
        ctx.raw_out() << "}\n";
        ctx.raw_out() << ".bingo-card .failed {\n";
        ctx.raw_out() << "\tbackground-color: #800;\n";
        ctx.raw_out() << "\tcolor: #eee;\n";
        ctx.raw_out() << "}\n";
        return true;
    }

    std::string path1 = path;
    if (path1.find("/?refresh=") == 0)
    {
        const size_t len = strlen("/?refresh=");
        if (path1.find_first_not_of("0123456789", len) == std::string::npos)
        {
            path1.erase(0, len);
            ctx.headers()["Refresh"] = path1;
            path1.clear();
        }
    }

    if (!path1.empty())
    {
        return false;
    }

    ctx.raw_out() << "<title>Dwarf Fortress Bingo</title>\n";
    ctx.raw_out() << "<link href=\"/style.css\" rel=\"stylesheet\">\n";
    ctx.raw_out() << "<link href=\"/bingo/style.css\" rel=\"stylesheet\">\n";
    if (!active_card)
    {
        ctx.raw_out() << "<i>no bingo currently in progress</i>\n";
        return true;
    }

    ctx.raw_out() << "<table class=\"bingo-card\">\n";
    ctx.raw_out() << "<caption><b>";
    ctx.raw_out() << clean_string(active_card->summarize(), false);
    ctx.raw_out() << "</b><br>";
    ctx.raw_out() << clean_string(active_card->describe(), true);
    ctx.raw_out() << "</caption>\n";
    for (auto & row : active_card->squares)
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
            ctx.raw_out() << "\"><div><b>";
            ctx.raw_out() << clean_string(square.summarize(), false);
            ctx.raw_out() << "</b><br>";
            ctx.raw_out() << clean_string(square.describe(), true);
            ctx.raw_out() << "</div></td>\n";
        }
        ctx.raw_out() << "</tr>\n";
    }
    ctx.raw_out() << "</table>\n";

    return true;
}
