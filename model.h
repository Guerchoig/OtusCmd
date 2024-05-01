/// @brief A data model module
///
#pragma once
#include <string>
#include <time.h>
#include <vector>

/// @brief Enum for lexemas
enum Lex
{
    OpenBr = 0,    // open bracket
    CloseBr,       // close bracket
    Cmd,           // command
    StartOfStatic, // start of parsing a static commands bulk
    EndOfInput     // end sym
};

/// @brief Stores a bulk: a header and a vector for command strings
struct bulk_t
{

    clock_t timestamp = 0;         // time for naming a file
    std::vector<std::string> cmds; // command strings
    enum Lex lex;                  // is OpenBr for 'dynamic' bulks and StartOfStatic
    size_t target_size = 0;        // 0 for dynamic bulks and block_size for static ones

    bulk_t(clock_t timestamp, enum Lex lex, size_t target_size) : timestamp(timestamp), lex(lex), target_size(target_size) {}
};
