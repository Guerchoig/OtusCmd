/// @brief IO module for console i/o and files output only
///
#pragma once
#include "model.h"
#include <iostream>
#include <fstream>

/// @brief Envelope for std::getline()
/// @param buf - string buffer
void c_get_line(std::string &buf)
{
    try
    {
        std::getline(std::cin, buf);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

/// @brief Envelope for cin.eof()
/// @return
bool c_eof()
{
    return std::cin.eof();
}

/// @brief A bulk output to a stream. Exception are treated in the calling function
/// @return
void write_bulk_to_stream(const bulk_t &bulk, std::ostream &stream)
{
    stream << "bulk: ";
    bool start = true;
    for (auto s : bulk.cmds)
    {
        if (!start)
            stream << ", ";
        else
            start = false;
        stream << s;
    }
    stream << '\n';
}

/// @brief Outputs a bulk to console an to a file, calling twice
/// 'write_bulk_to_stream' with a different stream parameter
/// @param bulk cmd bulk to output
void output_bulk(const bulk_t &bulk)
{
    if (bulk.cmds.size() == 0)
        return;
    try
    {
        write_bulk_to_stream(bulk, std::cout);

        std::ofstream output(std::string("./log/bulk") + std::to_string(bulk.timestamp) + std::string(".log"), std::ios::binary);
        write_bulk_to_stream(bulk, output);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
