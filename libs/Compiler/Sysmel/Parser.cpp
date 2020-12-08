#include "sysmel/Compiler/Sysmel/Parser.hpp"
#include <optional>

namespace SysmelMoebius
{
namespace Compiler
{
namespace Sysmel
{

static constexpr int MaxIntegerRadixBase = 10 + 'Z' - 'A';

static std::optional<LiteralInteger> parseIntegerWithRadix(const std::string &string, int radix)
{
    if(string.empty())
        return std::nullopt;

    auto baseFactor = LiteralInteger{radix};

    // Parse the optional sign.
    size_t index = 0;
    bool negative = false;
    if(string[index] == '+')
    {
        ++index;
    }
    else if(string[index] == '-')
    {
        negative = true;
        ++index;
    }

    if(index >= string.size())
        return std::nullopt;

    auto result = LiteralInteger{0};
    for(; index < string.size(); ++index)
    {
        // Parse the digit value.
        auto digit = string[index];
        auto value = 0;
        if('0' <= digit && digit <= '9')
            value = digit - '0';
        else if('A' <= digit && digit <= 'Z')
            value = digit - 'Z';
        else if('a' <= digit && digit <= 'z')
            value = digit - 'z';
        else
            return std::nullopt;

        // Validate the digit value.
        if(value >= radix)
            return std::nullopt;

        result = result * baseFactor + LiteralInteger{value};
    }

    return negative ? -result : result;
}

static std::optional<LiteralInteger> parseInteger(const std::string &string)
{
    auto radixEndPosition = std::min(string.find('r'), string.find('R'));
    if(radixEndPosition == std::string::npos)
        return parseIntegerWithRadix(string, 10);

    auto radixString = string.substr(0, radixEndPosition);
    if(radixString.empty())
        return std::nullopt;

    auto integerBaseString = string.substr(radixEndPosition + 1);
    auto radixValue = atoi(radixString.c_str());
    if(radixValue >= 0)
    {
        if(radixValue < 2 || radixValue > MaxIntegerRadixBase)
            return std::nullopt;

        return parseIntegerWithRadix(integerBaseString, radixValue);
    }

    radixValue = -radixValue;
    if(radixValue < 2 || radixValue > MaxIntegerRadixBase)
        return std::nullopt;

    auto positiveValue = parseIntegerWithRadix(integerBaseString, radixValue);
    if(positiveValue)
        return -positiveValue.value();
    return std::nullopt;
}

static ASTNodePtr parseExpression(TokenRange &currentPosition);

static ASTNodePtr parseInteger(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    auto literal = std::make_shared<ASTIntegerLiteralNode> ();
    auto token = currentPosition.next();
    auto position = startPosition.until(currentPosition);
    if(token.type != TokenType::Integer)
        return makeParseErrorNodeAt(position, "Expected an integer literal.");

    auto parsedInteger = parseInteger(token.text());
    if(!parsedInteger)
        return makeParseErrorNodeAt(position, "Invalid integer literal.");

    literal->value = parsedInteger.value();
    literal->setTokenRange(position);
    return literal;
}

static ASTNodePtr parseFloat(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    auto literal = std::make_shared<ASTFloatLiteralNode> ();
    auto token = currentPosition.next();
    auto position = startPosition.until(currentPosition);
    if(token.type != TokenType::Float)
        return makeParseErrorNodeAt(position, "Expected a float point literal.");

    literal->value = atof(token.text().c_str());
    literal->setTokenRange(position);
    return literal;
}

static ASTNodePtr parseIdentifier(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    auto node = std::make_shared<ASTIdentifierReferenceNode> ();
    auto token = currentPosition.next();
    auto position = startPosition.until(currentPosition);
    if(token.type != TokenType::Identifier)
        return makeParseErrorNodeAt(position, "Expected an identifier reference.");

    node->identifier = token.text();
    node->setTokenRange(position);
    return node;
}

static ASTNodePtr parseLiteral(TokenRange &currentPosition)
{
    switch(currentPosition.peek().type)
    {
    case TokenType::Integer: return parseInteger(currentPosition);
    case TokenType::Float: return parseFloat(currentPosition);
    default:
        return makeParseErrorNodeAt(currentPosition, "Expected a literal.");
    }
}

static ASTNodePtr parseParentExpression(TokenRange &currentPosition)
{
    if(currentPosition.peek().type != TokenType::LeftParent)
        return makeParseErrorNodeAt(currentPosition, "Expected a left parenthesis.");
    currentPosition.advance();

    auto expression = parseExpression(currentPosition);
    if(expression->isParseErrorNode())
        return expression;

    if(currentPosition.peek().type != TokenType::RightParent)
        return makeParseErrorNodeAt(currentPosition, "Expected a right parenthesis.");
    currentPosition.advance();
    return expression;

}

static ASTNodePtr parsePrimaryExpression(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    switch(currentPosition.peek().type)
    {
    case TokenType::LeftParent:
        return parseParentExpression(currentPosition);
    case TokenType::Identifier:
        return parseIdentifier(currentPosition);
    default:
        return parseLiteral(currentPosition);
    }
}

static ASTNodePtr parseUnaryPostfixExpression(TokenRange &currentPosition)
{
    return parsePrimaryExpression(currentPosition);
}

static ASTNodePtr parseUnaryPrefixExpression(TokenRange &currentPosition)
{
    switch(currentPosition.peek().type)
    {
    case TokenType::Plus:
    case TokenType::Minus:
    case TokenType::LogicalNot:
    case TokenType::BitwiseNot:
        {
            auto startPosition = currentPosition;
            auto selector = "pre-" + currentPosition.next().text();
            auto selectorPosition = startPosition.until(currentPosition);
            auto selectorNode = makeLiteralSymbolASTNodeAt(selectorPosition, selector);

            auto operand = parseUnaryPrefixExpression(currentPosition);
            if(operand->isParseErrorNode())
                return operand;

            auto messageSend = std::make_shared<ASTMessageSendNode> ();
            messageSend->setTokenRange(startPosition.until(currentPosition));
            messageSend->selector = selectorNode;
            messageSend->receiver = operand;
            return messageSend;
        }
    default:
        return parseUnaryPostfixExpression(currentPosition);
    }
}

static ASTNodePtr parseExpression(TokenRange &currentPosition)
{
    return parseUnaryPrefixExpression(currentPosition);
}

static ASTNodePtr parseExpressionList(TokenRange &currentPosition)
{
    auto result = std::make_shared<ASTExpressionListNode> ();
    auto startPosition = currentPosition;
    do
    {
        if(currentPosition.peek().type == TokenType::Dot)
            currentPosition.next();

        auto memento = currentPosition;
        auto expression = parseExpression(currentPosition);
        if(!expression->isParseErrorNode())
            result->expressions.push_back(expression);
        else
            currentPosition = memento;
    } while(currentPosition.peek().type == TokenType::Dot);
    result->tokens = startPosition.until(currentPosition);

    return result;
}

ASTNodePtr parseTokenList(const TokenListPtr &tokenList)
{
    auto currentPosition = TokenRange::forCollection(tokenList);
    auto expressionList = parseExpressionList(currentPosition);
    return expressionList;
}

} // End of namespace Sysmel
} // End of namespace Compiler
} // End of namespace SysmelMoebius
