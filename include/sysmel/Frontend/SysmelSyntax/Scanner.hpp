#ifndef SYSMEL_COMPILER_PARSER_SCANNER_HPP
#define SYSMEL_COMPILER_PARSER_SCANNER_HPP
#pragma once

#include "Token.hpp"

namespace Sysmel
{
namespace Frontend
{
namespace SysmelSyntax
{

SYSMEL_COMPILER_LIB_EXPORT TokenListPtr scanSourceCollection(const SourceCollectionPtr &sourceCollection);

inline TokenListPtr scanString(const std::string &sourceString, const std::string &sourceName = "")
{
    return scanSourceCollection(std::make_shared<SourceCollection> (sourceString, sourceName));
}

} // End of namespace SysmelSyntax
} // End of namespace Frontend
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_PARSER_SCANNER_HPP
