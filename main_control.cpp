/// @brief This module contains command processing
/// and the main function

#include "model.h"
#include "inout.h"
#include <utility>
#include <stack>

/// @brief A type for lexemas: Enum + string
using lexema_t = std::pair<enum Lex, std::string>;

/// @brief The command stack
/// is used here for convenient creation and deletion of command blocks
/// stack depth is <= 1
using bulks_t = std::stack<bulk_t>;

/// @brief Global variable for size of "static" bulks
int bulk_size = 0;

/// @brief Returns next lexema from input
/// @return std::pair<enum Lex, std::string>
lexema_t next_lexema()
{
    std::string buf;

    c_get_line(buf);

    if (c_eof())
        return std::make_pair(EndOfInput, std::string(""));
    if (buf.find('{') != buf.npos)
        return std::make_pair(OpenBr, std::string("{"));
    if (buf.find('}') != buf.npos)
        return std::make_pair(CloseBr, std::string("}"));
    return std::make_pair(Cmd, buf);
}

/// @brief The main automate to process commands
/// @param bulks - one level stack of command bulks
void proceed(bulks_t &bulks)
{
    // Set initial state
    bulks.push(bulk_t(clock(), StartOfStatic, bulk_size));
    int is_dynamic = 0;

    while (true)
    {
        auto lexema = next_lexema();
        int lexlex = lexema.first; // first: lexema enum,
                                   // second: command string, if any, or ""

        switch (lexlex)
        {
        case OpenBr:          // '{'
            if (is_dynamic++) // nested '{' are only accounted
                break;
            output_bulk(bulks.top());

            // destroys the old cmd bulk
            bulks.pop();

            // Creates & constructs new cmd bulk header
            bulks.push(bulk_t(clock(), OpenBr, 0));
            break;
        case CloseBr:         // '}'
            if (--is_dynamic) // checks for nesting level
                break;
            output_bulk(bulks.top());
            bulks.pop();
            bulks.push(bulk_t(clock(), StartOfStatic, bulk_size));
            break;
        case Cmd: // command
            bulks.top().cmds.emplace_back(lexema.second);

            // for dynamic bulks target_size is 0, for static ones - is equal to bulk_size
            if (bulks.top().cmds.size() == bulks.top().target_size)
            {
                output_bulk(bulks.top());
                bulks.pop();
                bulks.push(bulk_t(clock(), StartOfStatic, bulk_size));
            }
            break;
        case EndOfInput: // ^D
            if (bulks.top().lex == OpenBr)
                return;
            output_bulk(bulks.top());
            return;
        };
    }
}

/// @brief Just a main proc
/// @param argc is not used, because the param number is known to be 1
/// @param argv nof commands in a bulk, is stored into a global var bulk_size
/// @return zero
int main([[maybe_unused]] int argc, char *argv[])
{
    bulks_t bulks;
    bulk_size = std::atoi(argv[1]);
    proceed(bulks);
    return 0;
}