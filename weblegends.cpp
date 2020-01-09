#include "bingo.h"
#include "weblegends-plugin.h"

std::string clean_string(std::string s, bool newlines)
{
    size_t i = s.find('&');
    while (i != std::string::npos)
    {
        s.replace(i, 1, "&amp;");
        i = s.find('&', i + 4);
    }

    i = s.find('<');
    while (i != std::string::npos)
    {
        s.replace(i, 1, "&lt;");
        i = s.find('<', i + 3);
    }

    i = s.find('>');
    while (i != std::string::npos)
    {
        s.replace(i, 1, "&gt;");
        i = s.find('>', i + 3);
    }

    if (newlines)
    {
        i = s.find('\n');
        while (i != std::string::npos)
        {
            s.replace(i, 1, "<br>");
            i = s.find('\n', i + 3);
        }
    }

    return s;
}

bool bingo_weblegends_handler(weblegends_handler_v1 & ctx, const std::string & path)
{
    if (path == "/style.css")
    {
        ctx.headers()["Content-Type"] = "text/css";
        ctx.raw_out() << ".bingo-card {\n";
        ctx.raw_out() << "\ttable-layout: fixed;\n";
        ctx.raw_out() << "\tborder-collapse: collapse;\n";
        ctx.raw_out() << "}\n";
        ctx.raw_out() << ".bingo-card td {\n";
        ctx.raw_out() << "\twidth: 150px;\n";
        ctx.raw_out() << "\theight: 150px;\n";
        ctx.raw_out() << "\tpadding: 10px;\n";
        ctx.raw_out() << "\ttext-align: center;\n";
        ctx.raw_out() << "\tborder: 1px solid #000;\n";
        ctx.raw_out() << "}\n";
        ctx.raw_out() << ".bingo-card .possible {\n";
        ctx.raw_out() << "\tbackground-color: #aaa;\n";
        ctx.raw_out() << "\tcolor: #000;\n";
        ctx.raw_out() << "}\n";
        ctx.raw_out() << ".bingo-card .succeeded {\n";
        ctx.raw_out() << "\tbackground-color: #6f6;\n";
        ctx.raw_out() << "\tcolor: #000;\n";
        ctx.raw_out() << "\tfont-weight: bold;\n";
        ctx.raw_out() << "}\n";
        ctx.raw_out() << ".bingo-card .failed {\n";
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
    ctx.raw_out() << "</b>: ";
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
            ctx.raw_out() << "\"><b>";
            ctx.raw_out() << clean_string(square.summarize(), false);
            ctx.raw_out() << "</b><br>";
            ctx.raw_out() << clean_string(square.describe(), true);
            ctx.raw_out() << "</td>\n";
        }
        ctx.raw_out() << "</tr>\n";
    }
    ctx.raw_out() << "</table>\n";

    return true;
}
