#ifndef SYSMEL_COMPILER_PARSER_PARSER_HPP
#define SYSMEL_COMPILER_PARSER_PARSER_HPP
#pragma once

#include "AST.hpp"
#include "Scanner.hpp"

namespace Sysmel
{
namespace Frontend
{
namespace SysmelSyntax
{

SYSMEL_COMPILER_LIB_EXPORT ASTNodePtr parseTokenList(const TokenListPtr &tokenList);
SYSMEL_COMPILER_LIB_EXPORT ASTNodePtr parseTokenListWithLiteralArrayContent(const TokenListPtr &tokenList);

inline ASTNodePtr parseString(const std::string &sourceString, const std::string &sourceName = "")
{
    return parseTokenList(scanString(sourceString, sourceName));
}

inline ASTNodePtr parseStringWithLiteralArrayContent(const std::string &sourceString, const std::string &sourceName = "")
{
    return parseTokenListWithLiteralArrayContent(scanString(sourceString, sourceName));
}

} // End of namespace SysmelSyntax
} // End of namespace Frontend
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_PARSER_PARSER_HPP
