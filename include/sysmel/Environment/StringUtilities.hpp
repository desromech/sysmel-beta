#ifndef SYSMEL_ENVIRONMENT_STRING_UTILITIES_HPP
#define SYSMEL_ENVIRONMENT_STRING_UTILITIES_HPP
#pragma once

#include <vector>
#include <ostream>
#include <sstream>
#include "../DLLInterface.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_COMPILER_LIB_EXPORT bool isValidIdentifierString(const std::string &string);

SYSMEL_COMPILER_LIB_EXPORT void formatUtf8Character(char c, std::ostream &out);
SYSMEL_COMPILER_LIB_EXPORT void formatUtf32Character(char32_t c, std::ostream &out);

SYSMEL_COMPILER_LIB_EXPORT std::string formatStringLiteral(const std::string &value);
SYSMEL_COMPILER_LIB_EXPORT std::string formatSymbolLiteral(const std::string &value);
SYSMEL_COMPILER_LIB_EXPORT std::string formatString(const std::string &format, const std::vector<std::string> &arguments);

SYSMEL_COMPILER_LIB_EXPORT std::string basename(const std::string &path);
SYSMEL_COMPILER_LIB_EXPORT std::string basenameWithoutExtension(const std::string &path);
SYSMEL_COMPILER_LIB_EXPORT std::string dirname(const std::string &path);
SYSMEL_COMPILER_LIB_EXPORT bool stringBeginsWith(const std::string &string, const std::string &prefix);
SYSMEL_COMPILER_LIB_EXPORT std::vector<std::string> split(const std::string &string, char delim = ' ');

template<typename T>
std::string castToString(const T &value)
{
    std::ostringstream out;
    out << value;
    return out.str();
}

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_STRING_UTILITIES_HPP