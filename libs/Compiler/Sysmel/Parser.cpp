#include "sysmel/Compiler/Sysmel/Parser.hpp"
#include <optional>

namespace SysmelMoebius
{
namespace Compiler
{
namespace Sysmel
{

static constexpr int MaxIntegerRadixBase = 10 + 'Z' - 'A';

static ASTNodePtr parseExpression(TokenRange &currentPosition);
static ASTNodePtr parseAssignmentExpression(TokenRange &currentPosition);

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

ASTNodePtrList parseCallArguments(TokenRange &currentPosition)
{
    ASTNodePtrList result;
    if(currentPosition.next().type != TokenType::LeftParent)
    {
        result.push_back(makeParseErrorNodeAt(currentPosition, "Expected call arguments."));
        return result;
    }

    // Empty case.
    if(currentPosition.peek().type == TokenType::RightParent)
    {
        currentPosition.next();
        return result;
    }

    // First element.
    auto firstArgument = parseAssignmentExpression(currentPosition);
    result.push_back(firstArgument);

    while(currentPosition.peek().type == TokenType::Comma)
    {
        currentPosition.next();

        auto argument = parseAssignmentExpression(currentPosition);
        result.push_back(argument);
        if(argument->isParseErrorNode())
            break;
    }

    if(currentPosition.peek().type == TokenType::RightParent)
    {
        currentPosition.next();
    }
    else
    {
        if(!result.back()->isParseErrorNode())
            result.push_back(makeParseErrorNodeAt(currentPosition, "Expected a right parenthesis."));
    }

    return result;
}

static ASTNodePtr parseUnaryPostfixExpression(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    auto receiver = parsePrimaryExpression(currentPosition);
    if(receiver->isParseErrorNode())
        return receiver;

    for(;;)
    {
        switch(currentPosition.peek().type)
        {
        case TokenType::Identifier:
            // Unary message
            {
                auto selector = std::make_shared<ASTSymbolLiteralNode> ();
                selector->setTokenRange(currentPosition.until(1));
                selector->value = currentPosition.next().text();

                auto messageSend = std::make_shared<ASTMessageSendNode> ();
                messageSend->receiver = receiver;
                messageSend->selector = selector;
                messageSend->setTokenRange(startPosition.until(currentPosition));
                receiver = messageSend;
            }
            break;
        case TokenType::LeftParent:
            // Call expression
            {
                auto selector = std::make_shared<ASTSymbolLiteralNode> ();
                selector->setTokenRange(currentPosition.until(1));
                selector->value = "()";

                auto messageSend = std::make_shared<ASTMessageSendNode> ();
                messageSend->selector = selector;
                messageSend->receiver = receiver;
                messageSend->arguments = parseCallArguments(currentPosition);
                receiver = messageSend;
            }
            break;
        case TokenType::LeftBracket:
            // Subscript expression.
            {

            }
            break;
        case TokenType::QuasiUnquote:
            // Unary message with macro selector.
            {

            }
            break;
        default:
            return receiver;
        }
    }
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

static inline int precedenceOfOperator(TokenType tokenType)
{
    switch(tokenType)
    {
    case TokenType::Times:
    case TokenType::Divide:
    case TokenType::IntegerDivide:
    case TokenType::Modulus:
        return 10;
    case TokenType::Plus:
    case TokenType::Minus:
        return 9;
    case TokenType::LogicalShiftLeft:
    case TokenType::LogicalShiftRight:
        return 8;

    case TokenType::LessThan:
    case TokenType::LessOrEqualThan:
    case TokenType::GreaterThan:
    case TokenType::GreaterOrEqualThan:
        return 7;

    case TokenType::Equality:
    case TokenType::IdentityEquality:
    case TokenType::NotEquality:
    case TokenType::IdentityNotEquality:
        return 6;

    case TokenType::BitwiseAnd: return 5;
    case TokenType::BitwiseXor: return 4;
    case TokenType::BitwiseOr: return 3;
    case TokenType::LogicalAnd: return 2;
    case TokenType::LogicalOr: return 1;
    case TokenType::GenericBinaryOperator: return 0;

    default: return -1;
    }
}

static ASTNodePtr parseBinaryExpressionWithPrecedence(ASTNodePtr leftOperand, TokenRange &currentPosition, int minimalPrecedence)
{
    // Algorithm from: https://en.wikipedia.org/wiki/Operator-precedence_parser
    int currentPrecedence;
    while((currentPrecedence = precedenceOfOperator(currentPosition.peek().type)) >= minimalPrecedence)
    {
        // Make the selector symbol node.
        auto selectorPosition = currentPosition.until(1);
        auto operatorToken = currentPosition.next();
        auto selectorNode = makeLiteralSymbolASTNodeAt(selectorPosition, operatorToken.text());

        // Parse the next operand
        auto rightOperand = parseUnaryPrefixExpression(currentPosition);

        // Apply the next precedence levels.
        int nextPrecedence;
        while((nextPrecedence = precedenceOfOperator(currentPosition.peek().type)) > currentPrecedence)
        {
            rightOperand = parseBinaryExpressionWithPrecedence(rightOperand, currentPosition, nextPrecedence);
        }

        // Make the node.
        auto messageSend = std::make_shared<ASTMessageSendNode> ();
        messageSend->setTokenRange(leftOperand->tokens.until(rightOperand->tokens));
        messageSend->selector = selectorNode;
        messageSend->receiver = leftOperand;
        messageSend->arguments.push_back(rightOperand);
        leftOperand = messageSend;
    }

    return leftOperand;
}

static ASTNodePtr parseBinaryExpression(TokenRange &currentPosition)
{
    auto primary = parseUnaryPrefixExpression(currentPosition);
    if(primary->isParseErrorNode())
        return primary;

    return parseBinaryExpressionWithPrecedence(primary, currentPosition, 0);
}

static ASTNodePtr parseAssignmentExpression(TokenRange &currentPosition)
{
    return parseBinaryExpression(currentPosition);
}

static ASTNodePtr parseExpression(TokenRange &currentPosition)
{
    return parseAssignmentExpression(currentPosition);
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
