#ifndef SYSMEL_ENVIRONMENT_STRING_UTILITIES_HPP
#define SYSMEL_ENVIRONMENT_STRING_UTILITIES_HPP
#pragma once

#include <vector>
#include <ostream>
#include <sstream>

namespace Sysmel
{
namespace Environment
{

bool isValidIdentifierString(const std::string &string);

void formatUtf8Character(char c, std::ostream &out);
void formatUtf32Character(char32_t c, std::ostream &out);

std::string formatStringLiteral(const std::string &value);
std::string formatSymbolLiteral(const std::string &value);
std::string formatString(const std::string &format, const std::vector<std::string> &arguments);

std::string basename(const std::string &path);
std::string basenameWithoutExtension(const std::string &path);
std::string dirname(const std::string &path);
bool stringBeginsWith(const std::string &string, const std::string &prefix);
std::vector<std::string> split(const std::string &string, char delim = ' ');

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