#ifndef SYSMEL_COMPILER_PARSER_SCANNER_HPP
#define SYSMEL_COMPILER_PARSER_SCANNER_HPP
#pragma once

#include "Token.hpp"

namespace SysmelMoebius
{
namespace Compiler
{
namespace Sysmel
{

TokenList scanSourceCollection(const SourceCollectionPtr &sourceCollection);

inline TokenList scanString(const std::string &sourceString, const std::string &sourceName = "")
{
    return scanSourceCollection(std::make_shared<SourceCollection> (sourceString, sourceName));
}

} // End of namespace Sysmel
} // End of namespace Compiler
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_PARSER_SCANNER_HPP
