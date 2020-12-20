#ifndef SYSMEL_BASE_COMPILER_PARSER_SCANNER_HPP
#define SYSMEL_BASE_COMPILER_PARSER_SCANNER_HPP
#pragma once

#include "Token.hpp"

namespace SysmelMoebius
{
namespace Compiler
{
namespace Sysmel
{

TokenListPtr scanSourceCollection(const SourceCollectionPtr &sourceCollection);

inline TokenListPtr scanString(const std::string &sourceString, const std::string &sourceName = "")
{
    return scanSourceCollection(std::make_shared<SourceCollection> (sourceString, sourceName));
}

} // End of namespace Sysmel
} // End of namespace Compiler
} // End of namespace SysmelMoebius

#endif //SYSMEL_BASE_COMPILER_PARSER_SCANNER_HPP
