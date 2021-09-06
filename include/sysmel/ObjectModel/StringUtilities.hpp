#ifndef SYSMEL_COMPILER_OBJECT_MODEL_STRING_UTILITIES_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_STRING_UTILITIES_HPP
#pragma once

#include <ostream>
#include <iomanip>

namespace SysmelMoebius
{
namespace ObjectModel
{

inline void formatUtf8Character(char c, std::ostream &out)
{
    switch(c)
    {
    case 0:
        out << "\\0";
        break;
    case '\r':
        out << "\\r";
        break;
    case '\n':
        out << "\\n";
        break;
    case '\t':
        out << "\\t";
        break;
    case '\\':
        out << "\\\\";
        break;
    default:
        out << c;
        break;
    }
}

inline void formatUtf32Character(char32_t c, std::ostream &out)
{
    if(c <= 127)
    {
        formatUtf8Character(char(c), out);
    }
    else
    {
        if(c <= 0xFFFF)
            out << "\\u" << std::hex << std::setfill('0') << std::setw(4) << uint32_t(c);
        else
            out << "\\U" << std::hex << std::setfill('0') << std::setw(8) << uint32_t(c);
        out << std::dec << std::setw(0);
    }
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_STRING_UTILITIES_HPP