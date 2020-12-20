#ifndef SYSMEL_BASE_COMPILER_PARSER_TOKEN_HPP
#define SYSMEL_BASE_COMPILER_PARSER_TOKEN_HPP
#pragma once

#include "sysmel/BaseCompiler/SourcePosition.hpp"
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

    bool isErrorUnexpected() const
    {
        return type == TokenType::ErrorUnexpected;
    }

    bool isError() const
    {
        return TokenType::Error <= type && type <= TokenType::ErrorCount;
    }

    bool isEndOfSource() const
    {
        return type == TokenType::EndOfSource;
    }

    std::string text() const
    {
        return sourcePosition.text();
    }
};

typedef std::vector<Token> TokenList;
typedef std::shared_ptr<TokenList> TokenListPtr;

/**
 * I am a range of tokens in a token stream.
 */
struct TokenRange : CollectionRange<TokenRange, TokenListPtr, TokenList, Token>
{
    typedef Token peek_type;

    Token eofValue() const
    {
        if(!collection->empty() && collection->back().isEndOfSource())
            return collection->back();
        if(!collection->empty())
        {
            auto sourceCollection = collection->front().sourcePosition.collection;
            if(sourceCollection)
                return Token{TokenType::EndOfSource, SourcePosition::forEndOfCollection(sourceCollection)};
        }

        return Token{TokenType::EndOfSource, SourcePosition()};
    }

    SourcePosition asSourcePosition() const
    {
        if(startPosition + 1 >= endPosition)
            return front().sourcePosition;
        return front().sourcePosition.until(back().sourcePosition);
    }
};

} // End of namespace Sysmel
} // End of namespace Compiler
} // End of namespace SysmelMoebius

#endif //SYSMEL_BASE_COMPILER_PARSER_TOKEN_HPP
