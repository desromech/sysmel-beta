#include "sysmel/Compiler/Sysmel/Scanner.hpp"

namespace SysmelMoebius
{
namespace Compiler
{
namespace Sysmel
{

static void skipLineComment(SourcePosition &currentPosition)
{
    while(!currentPosition.empty())
    {
        auto next = currentPosition.next();
        if(next == '\r' || next == '\n')
            return;
    }
}

static bool skipMultiLineComment(SourcePosition &currentPosition)
{
    while(!currentPosition.empty())
    {
        auto next = currentPosition.peek();
        currentPosition.advance();
        if(next == '*')
        {
            if(currentPosition.peek() == '#')
            {
                currentPosition.advance();
                return true;
            }
        }
    }

    return false;
}

static bool skipWhite(SourcePosition &currentPosition)
{
    while(!currentPosition.empty())
    {
        auto next = currentPosition.peek(0);
        if(next <= ' ')
        {
            currentPosition.advance();
            continue;
        }

        if(next == '#')
        {
            auto commentType = currentPosition.peek(1);
            if(commentType == '#')
            {
                skipLineComment(currentPosition);
                continue;
            }
            else if(commentType == '*')
            {
                auto memento = currentPosition;
                if(!skipMultiLineComment(currentPosition))
                {
                    currentPosition = memento;
                    return false;
                }
                continue;
            }
        }

        return true;
    }

    return true;
}

static inline bool isDigit(int c)
{
    return '0' <= c && c <= '9';
}

static inline bool isSign(int c)
{
    return '+' == c || '-' == c;
}

static inline bool isExponent(int c)
{
    return 'e' == c || 'E' == c;
}

static inline bool isRadix(int c)
{
    return 'r' == c || 'R' == c;
}

static inline bool isAlpha(int c)
{
    return ('A' <= c && c <= 'Z') ||
        ('a' <= c && c <= 'z');
}

static inline bool isAlphaNumeric(int c)
{
    return isAlpha(c) || isDigit(c);
}

static inline bool isIdentifierStart(int c)
{
    return isAlpha(c) ||
        '_' == c;
}

static inline bool isIdentifierCharacter(int c)
{
    return isIdentifierStart(c) || isDigit(c);
}

static TokenType skipKeywordOrIdentifier(SourcePosition &currentPosition)
{
    bool scanningKeyword = false;
    auto lastKeywordEndPosition = currentPosition;

    do
    {
        currentPosition.skipWhile(isIdentifierCharacter);
        if(currentPosition.peek() == ':')
        {
            currentPosition.advance();
            scanningKeyword = true;
            lastKeywordEndPosition = currentPosition;
        }
        else
        {
            if(scanningKeyword)
            {
                currentPosition = lastKeywordEndPosition;
                return TokenType::Keyword;
            }
        }

    } while(scanningKeyword && isIdentifierStart(currentPosition.peek()));

    return scanningKeyword ? TokenType::Keyword : TokenType::Identifier;
}

static TokenType skipNumber(SourcePosition &currentPosition)
{
    // Skip the sign;
    if(isSign(currentPosition.peek()))
        currentPosition.advance();

    // Skip the integer or radix part.
    currentPosition.skipWhile(isDigit);

    auto dotExponentOrRadix = currentPosition.peek();
    if(isRadix(dotExponentOrRadix))
    {
        // Radix integer
        currentPosition.skipWhile(isAlphaNumeric);
        return TokenType::Integer;
    }

    bool isFloat = false;

    // Fractional part.
    if(dotExponentOrRadix == '.')
    {
        currentPosition.advance();
        isFloat = true;
        currentPosition.skipWhile(isDigit);

        dotExponentOrRadix = currentPosition.peek();
    }

    // Float exponent.
    if(isExponent(dotExponentOrRadix))
    {
        currentPosition.advance();
        isFloat = true;
        if(isSign(currentPosition.peek()))
            currentPosition.advance();

        currentPosition.skipWhile(isDigit);
    }

    return isFloat ? TokenType::Float : TokenType::Integer;
}

static void scanNextToken(SourcePosition &currentPosition, TokenList &result)
{
    if(!skipWhite(currentPosition))
    {
        // This may fail due to an unclosed multiline comment.
        result.push_back(Token {TokenType::Error, currentPosition});
        currentPosition.advanceToEnd();
        return;
    }

    if(currentPosition.empty())
    {
        result.push_back(Token {TokenType::EndOfSource, currentPosition});
        return;
    }

    auto tokenStart = currentPosition;
    auto tokenFirst = currentPosition.peek();

    // Numbers
    if(isDigit(tokenFirst) || (isSign(tokenFirst) && isDigit(currentPosition.peek(1))))
    {
        auto tokenType = skipNumber(currentPosition);
        result.push_back(Token {tokenType, tokenStart.until(currentPosition)});
        return;
    }

    // Identifiers and keywords.
    if(isIdentifierStart(tokenFirst))
    {
        auto tokenType = skipKeywordOrIdentifier(currentPosition);
        result.push_back(Token {tokenType, tokenStart.until(currentPosition)});
        return;
    }

    // Unrecognized token, emit an error.
    if(!result.empty() && result.back().isError())
    {
        currentPosition.advance(1);
        auto &lastErrorPosition = result.back().sourcePosition;
        lastErrorPosition.endPosition = std::max(lastErrorPosition.endPosition, currentPosition.startPosition);
    }
    else
    {
        result.push_back(Token {TokenType::Error, currentPosition.until(1)});
        currentPosition.advance(1);
    }
}

TokenList scanSourceCollection(const SourceCollectionPtr &sourceCollection)
{
    TokenList result;
    auto currentPosition = SourcePosition::forSourceCollection(sourceCollection);
    do
    {
        scanNextToken(currentPosition, result);
    } while (!currentPosition.empty());

    return result;
}

} // End of namespace Sysmel
} // End of namespace Compiler
} // End of namespace SysmelMoebius
