#ifndef SYSMEL_COMPILER_PARSER_PARSER_HPP
#define SYSMEL_COMPILER_PARSER_PARSER_HPP
#pragma once

#include "AST.hpp"
#include "Scanner.hpp"

namespace SysmelMoebius
{
namespace Compiler
{
namespace Sysmel
{

ASTNodePtr parseTokenList(const TokenListPtr &tokenList);
inline ASTNodePtr parseString(const std::string &sourceString, const std::string &sourceName = "")
{
    return parseTokenList(scanString(sourceString, sourceName));
}

} // End of namespace Sysmel
} // End of namespace Compiler
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_PARSER_PARSER_HPP
