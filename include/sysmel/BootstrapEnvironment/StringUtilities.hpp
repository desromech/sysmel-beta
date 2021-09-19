#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_STRING_UTILITIES_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_STRING_UTILITIES_HPP
#pragma once

#include <vector>
#include <ostream>
#include <iomanip>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

void formatUtf8Character(char c, std::ostream &out);
void formatUtf32Character(char32_t c, std::ostream &out);

std::string formatStringLiteral(const std::string &value);
std::string formatSymbolLiteral(const std::string &value);
std::string formatString(const std::string &format, const std::vector<std::string> &arguments);

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_STRING_UTILITIES_HPP