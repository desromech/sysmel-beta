#ifndef SYSMEL_COMPILER_PARSER_TOKEN_HPP
#define SYSMEL_COMPILER_PARSER_TOKEN_HPP
#pragma once

#include "sysmel/Compiler/SourcePosition.hpp"
#include <vector>

namespace SysmelMoebius
{
namespace Compiler
{
namespace Sysmel
{

/**
 * I represent a specific token type in the sysmel grammar.
 */
enum class TokenType
{
#define TOKEN_NAME(T) T,
#include "Token.inc"
#undef TOKEN_NAME
};

inline const char *tokenTypeToString(TokenType type)
{
    static const char *tokenNames[] = {
        #define TOKEN_NAME(T) #T,
        #include "Token.inc"
        #undef TOKEN_NAME
    };
    return tokenNames[int(type)];
}

inline std::ostream &operator<<(std::ostream &out, TokenType tokenType)
{
    out << tokenTypeToString(tokenType);
    return out;
}

/**
 * I am a token in a sysmel source file.
 */
struct Token
{
    TokenType type;
    SourcePosition sourcePosition;

    bool isError() const
    {
        return type == TokenType::Error;
    }

    std::string text() const
    {
        return sourcePosition.text();
    }
};

typedef std::vector<Token> TokenList;

} // End of namespace Sysmel
} // End of namespace Compiler
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_PARSER_TOKEN_HPP
