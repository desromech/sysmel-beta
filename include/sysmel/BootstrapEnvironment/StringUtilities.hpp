#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_STRING_UTILITIES_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_STRING_UTILITIES_HPP
#pragma once

#include <vector>
#include <ostream>
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

bool isValidIdentifierString(const std::string &string);

void formatUtf8Character(char c, std::ostream &out);
void formatUtf32Character(char32_t c, std::ostream &out);

std::string formatStringLiteral(const std::string &value);
std::string formatSymbolLiteral(const std::string &value);
std::string formatString(const std::string &format, const std::vector<std::string> &arguments);

template<typename T>
std::string castToString(const T &value)
{
    std::ostringstream out;
    out << value;
    return out.str();
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_STRING_UTILITIES_HPP