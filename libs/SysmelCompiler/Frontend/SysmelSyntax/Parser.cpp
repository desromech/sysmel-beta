#include "Frontend/SysmelSyntax/Parser.hpp"
#include <optional>
#include <cassert>

namespace Sysmel
{
namespace Frontend
{
namespace SysmelSyntax
{

static constexpr int MaxIntegerRadixBase = 10 + 'Z' - 'A';

static ASTNodePtr parseLiteralArray(TokenRange &currentPosition);
static ASTNodePtr parseDictionaryExpression(TokenRange &currentPosition, bool isLiteralDictionary);
static ASTNodePtr parseSourceWithLiteralArrayContent(TokenRange &currentPosition);
static ASTNodePtr parseLiteral(TokenRange &currentPosition);
static ASTNodePtr parseExpression(TokenRange &currentPosition);
static ASTNodePtr parseExpressionList(TokenRange &currentPosition);
static ASTNodePtr parsePrimaryExpression(TokenRange &currentPosition);
static ASTNodePtr parseQuasiUnquote(TokenRange &currentPosition);
static ASTNodePtr parseBinaryExpression(TokenRange &currentPosition);
static ASTNodePtr parseAssignmentExpression(TokenRange &currentPosition);

static bool isSingleKeyword(const std::string &keyword)
{
    size_t colonCount = 0;
    for(auto c : keyword)
    {
        if(c == ':')
            ++colonCount;
    }

    return colonCount == 1;
}
static std::optional<LargeInteger> parseIntegerWithRadix(const std::string &string, int radix)
{
    if(string.empty())
        return std::nullopt;

    auto baseFactor = LargeInteger{radix};

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

    auto result = LargeInteger{0};
    for(; index < string.size(); ++index)
    {
        // Parse the digit value.
        auto digit = string[index];
        auto value = 0;
        if('0' <= digit && digit <= '9')
            value = digit - '0';
        else if('A' <= digit && digit <= 'Z')
            value = digit - 'A' + 10;
        else if('a' <= digit && digit <= 'z')
            value = digit - 'a' + 10;
        else
            return std::nullopt;

        // Validate the digit value.
        if(value >= radix)
            return std::nullopt;

        result *= baseFactor;
        result += LargeInteger{value};
    }

    return negative ? -result : result;
}

static std::optional<LargeInteger> parseInteger(const std::string &string)
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

static ASTNodePtr parseIdentifierAsSymbol(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    auto node = std::make_shared<ASTSymbolLiteralNode> ();
    auto token = currentPosition.next();
    auto position = startPosition.until(currentPosition);
    if(token.type != TokenType::Identifier)
        return makeParseErrorNodeAt(position, "Expected an identifier.");

    node->value = token.text();
    node->setTokenRange(position);
    return node;
}

static bool parseStringEscapeSequences(const std::string &string, std::string &outResult, std::string &outError)
{
    outResult.reserve(string.size());

    for(size_t i = 0; i < string.size(); ++i)
    {
        auto c = string[i];
        if(c != '\\')
        {
            outResult.push_back(c);
            continue;
        }

        if(i + 1 >= string.size())
        {
            outError = "String literal has incomplete escape sequence.";
            return false;
        }

        c = string[++i];

        switch(c)
        {
        case '0':
            outResult.push_back(0);
            break;
        case 'n':
            outResult.push_back('\n');
            break;
        case 'r':
            outResult.push_back('\r');
            break;
        case 't':
            outResult.push_back('\t');
            break;
        default:
            outResult.push_back(c);
            break;
        }
    }
    return true;
}

static ASTNodePtr parseString(TokenRange &currentPosition)
{
    if(currentPosition.peek().type != TokenType::String)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a literal string.");

    auto startPosition = currentPosition;
    auto rawStringValue = currentPosition.next().text();
    assert(rawStringValue.size() >= 2);
    auto rawStringContent = rawStringValue.substr(1, rawStringValue.size() - 2);

    auto node = std::make_shared<ASTStringLiteralNode> ();
    node->setTokenRange(startPosition.until(currentPosition));

    std::string errorMessage;
    if(!parseStringEscapeSequences(rawStringContent, node->value, errorMessage))
        return makeParseErrorNodeAtToken(startPosition, errorMessage);

    return node;
}

static ASTNodePtr parseCharacter(TokenRange &currentPosition)
{
    if(currentPosition.peek().type != TokenType::Character)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a literal symbol string.");

    auto startPosition = currentPosition;
    auto rawStringValue = currentPosition.next().text();
    assert(rawStringValue.size() >= 2);
    auto rawStringContent = rawStringValue.substr(1, rawStringValue.size() - 2);

    auto node = std::make_shared<ASTCharacterLiteralNode> ();
    node->setTokenRange(startPosition.until(currentPosition));

    std::string stringContent;
    std::string errorMessage;
    if(!parseStringEscapeSequences(rawStringContent, stringContent, errorMessage))
        return makeParseErrorNodeAtToken(startPosition, errorMessage);

    // FIXME: Support the utf-8 encoding.
    if(stringContent.size() != 1)
        return makeParseErrorNodeAtToken(startPosition, "Invalid single character literal.");
    node->value = stringContent[0];
    return node;
}

static ASTNodePtr parseSimpleLiteralSymbol(TokenRange &currentPosition, size_t prefixSize)
{
    auto node = std::make_shared<ASTSymbolLiteralNode> ();
    node->setTokenRange(currentPosition.until(1));

    auto rawSymbolValue = currentPosition.next().text();
    assert(rawSymbolValue.size() > prefixSize);
    node->value = rawSymbolValue.substr(prefixSize);
    return node;
}

static ASTNodePtr parseLiteralSymbolKeyword(TokenRange &currentPosition)
{
    if(currentPosition.peek().type != TokenType::SymbolKeyword)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a literal keyword symbol.");

    return parseSimpleLiteralSymbol(currentPosition, 1);
}

static ASTNodePtr parseLiteralSymbolIdentifier(TokenRange &currentPosition)
{
    if(currentPosition.peek().type != TokenType::SymbolIdentifier)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a literal identifier symbol.");

    return parseSimpleLiteralSymbol(currentPosition, 1);
}

static ASTNodePtr parseLiteralSymbolOperator(TokenRange &currentPosition)
{
    if(currentPosition.peek().type != TokenType::SymbolOperator)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a literal operator symbol.");

    return parseSimpleLiteralSymbol(currentPosition, 1);
}

static ASTNodePtr parseLiteralSymbolString(TokenRange &currentPosition)
{
    if(currentPosition.peek().type != TokenType::SymbolString)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a literal symbol string.");

    auto startPosition = currentPosition;
    auto rawStringValue = currentPosition.next().text();
    assert(rawStringValue.size() >= 3);
    auto rawStringContent = rawStringValue.substr(2, rawStringValue.size() - 3);

    auto node = std::make_shared<ASTSymbolLiteralNode> ();
    node->setTokenRange(startPosition.until(currentPosition));

    std::string errorMessage;
    if(!parseStringEscapeSequences(rawStringContent, node->value, errorMessage))
        return makeParseErrorNodeAtToken(startPosition, errorMessage);

    return node;
}

static ASTNodePtr parseLiteralArrayElement(TokenRange &currentPosition)
{
    switch(currentPosition.peek().type)
    {
    case TokenType::LeftParent: return parseLiteralArray(currentPosition);
    case TokenType::Keyword: return parseSimpleLiteralSymbol(currentPosition, 0);
    case TokenType::Identifier: return parseIdentifier(currentPosition);
    case TokenType::DictionaryLeftBracket: return parseDictionaryExpression(currentPosition, true);

    case TokenType::Times:
    case TokenType::Divide:
    case TokenType::IntegerDivide:
    case TokenType::Modulus:
    case TokenType::Plus:
    case TokenType::Minus:
    case TokenType::LogicalShiftLeft:
    case TokenType::LogicalShiftRight:
    case TokenType::LessOrEqualThan:
    case TokenType::GreaterOrEqualThan:
    case TokenType::LessThan:
    case TokenType::GreaterThan:
    case TokenType::Equality:
    case TokenType::NotEquality:
    case TokenType::IdentityEquality:
    case TokenType::IdentityNotEquality:
    case TokenType::BitwiseAnd:
    case TokenType::BitwiseXor:
    case TokenType::BitwiseOr:
    case TokenType::BitwiseNot:
    case TokenType::LogicalAnd:
    case TokenType::LogicalOr:
    case TokenType::LogicalNot:
    case TokenType::GenericBinaryOperator:
        return parseSimpleLiteralSymbol(currentPosition, 0);

    default: return parseLiteral(currentPosition);
    }
}

static ASTNodePtr parseLiteralArray(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    {
        auto type = currentPosition.peek().type;
        if(type != TokenType::LiteralArrayLeftParent && type != TokenType::LeftParent)
            return makeParseErrorNodeAtToken(currentPosition, "Expected a literal array.");
        currentPosition.advance();
    }

    auto literalArray = std::make_shared<ASTLiteralArrayNode> ();
    while(currentPosition.peek().type != TokenType::RightParent)
    {
        auto element = parseLiteralArrayElement(currentPosition);
        literalArray->elements.push_back(element);
        if(element->isParseErrorNode())
        {
            literalArray->setTokenRange(startPosition.until(currentPosition));
            return literalArray;
        }
    }

    if(currentPosition.peek().type != TokenType::RightParent)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a right parenthesis.");
    currentPosition.advance();
    literalArray->setTokenRange(startPosition.until(currentPosition));
    return literalArray;
}

static ASTNodePtr parseSourceWithLiteralArrayContent(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;

    auto literalArray = std::make_shared<ASTLiteralArrayNode> ();
    while(currentPosition.peek().type != TokenType::EndOfSource)
    {
        auto element = parseLiteralArrayElement(currentPosition);
        literalArray->elements.push_back(element);
        if(element->isParseErrorNode())
        {
            literalArray->setTokenRange(startPosition.until(currentPosition));
            return literalArray;
        }
    }

    if(currentPosition.peek().type != TokenType::EndOfSource)
        return makeParseErrorNodeAtToken(currentPosition, "Expected the end of source.");
    currentPosition.advance();
    literalArray->setTokenRange(startPosition.until(currentPosition));
    return literalArray;
}

static ASTNodePtr parseLiteral(TokenRange &currentPosition)
{
    switch(currentPosition.peek().type)
    {
    case TokenType::Integer: return parseInteger(currentPosition);
    case TokenType::Float: return parseFloat(currentPosition);
    case TokenType::String: return parseString(currentPosition);
    case TokenType::Character: return parseCharacter(currentPosition);
    case TokenType::SymbolKeyword: return parseLiteralSymbolKeyword(currentPosition);
    case TokenType::SymbolIdentifier: return parseLiteralSymbolIdentifier(currentPosition);
    case TokenType::SymbolOperator: return parseLiteralSymbolOperator(currentPosition);
    case TokenType::SymbolString: return parseLiteralSymbolString(currentPosition);
    case TokenType::LiteralArrayLeftParent: return parseLiteralArray(currentPosition);
    default:
        return makeParseErrorNodeAtToken(currentPosition, "Expected a literal.");
    }
}

static ASTNodePtr parseParentExpression(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    if(currentPosition.peek().type != TokenType::LeftParent)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a left parenthesis.");
    currentPosition.advance();

    // Check for the empty tuple.
    if(currentPosition.peek().type == TokenType::RightParent)
    {
        currentPosition.advance();
        auto node = std::make_shared<ASTMakeTupleNode> ();
        node->setTokenRange(startPosition.until(currentPosition));
        return node;
    }

    auto expression = parseExpression(currentPosition);
    if(expression->isParseErrorNode())
        return expression;

    if(currentPosition.peek().type != TokenType::RightParent)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a right parenthesis.");
    currentPosition.advance();
    return expression;

}

static ASTNodePtr parseQuote(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    if(currentPosition.peek().type != TokenType::Quote)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a macro quote (`').");
    currentPosition.advance();

    auto quoted = parsePrimaryExpression(currentPosition);

    auto result = std::make_shared<ASTQuoteNode> ();
    result->quoted = quoted;
    result->setTokenRange(startPosition.until(currentPosition));
    return result;
}

static ASTNodePtr parseQuasiQuote(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    if(currentPosition.peek().type != TokenType::QuasiQuote)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a quasi quote (``).");
    currentPosition.advance();

    auto quoted = parsePrimaryExpression(currentPosition);

    auto result = std::make_shared<ASTQuasiQuoteNode> ();
    result->quoted = quoted;
    result->setTokenRange(startPosition.until(currentPosition));
    return result;
}

static ASTNodePtr parseQuasiUnquote(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    if(currentPosition.peek().type != TokenType::QuasiUnquote)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a quasi unquote (`').");
    currentPosition.advance();

    auto expression = parsePrimaryExpression(currentPosition);

    auto result = std::make_shared<ASTQuasiUnquoteNode> ();
    result->expression = expression;
    result->setTokenRange(startPosition.until(currentPosition));
    return result;
}

static ASTNodePtr parseSplice(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    if(currentPosition.peek().type != TokenType::Splice)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a splice (`@).");
    currentPosition.advance();

    auto expression = parsePrimaryExpression(currentPosition);

    auto result = std::make_shared<ASTSpliceNode> ();
    result->expression = expression;
    result->setTokenRange(startPosition.until(currentPosition));
    return result;
}

static void parseKeywordPragmaContent(TokenRange &currentPosition, ASTPragmaNode &pragma)
{
    std::string selectorValue;
    auto selectorStartPosition = currentPosition;

    // Parse the pragme keyword and arguments.
    while(currentPosition.peek().type == TokenType::Keyword)
    {
        auto keywordPosition = currentPosition.until(1);
        auto keyword = currentPosition.next().text();
        selectorValue += keyword;

        if(!isSingleKeyword(keyword))
            pragma.arguments.push_back(makeParseErrorNodeAtToken(keywordPosition, "Expected a single keyword."));

        auto argument = parsePrimaryExpression(currentPosition);
        pragma.arguments.push_back(argument);
        if(argument->isParseErrorNode())
            break;
    }

    // Set the selector value.
    auto selector = std::make_shared<ASTSymbolLiteralNode> ();
    selector->value = selectorValue;
    selector->setTokenRange(selectorStartPosition.until(currentPosition));
    pragma.selector = selector;
}

static ASTNodePtr parsePragma(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    if(currentPosition.peek().type != TokenType::LessThan)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a quasi unquote (`').");
    currentPosition.advance();

    auto pragma = std::make_shared<ASTPragmaNode> ();
    switch(currentPosition.peek().type)
    {
    case TokenType::Identifier:
        {
            auto selector = std::make_shared<ASTSymbolLiteralNode> ();
            selector->setTokenRange(currentPosition.until(1));
            selector->value = currentPosition.next().text();
            pragma->selector = selector;
        }
        break;
    case TokenType::Keyword:
        parseKeywordPragmaContent(currentPosition, *pragma);
        break;
    default:
        return makeParseErrorNodeAtToken(currentPosition, "Expected a pragma selector.");
    }

    if(currentPosition.peek().type == TokenType::GreaterThan)
    {
        currentPosition.advance();
    }
    else
    {
        if(pragma->arguments.empty() || !pragma->arguments.back()->isParseErrorNode())
            pragma->arguments.push_back(makeParseErrorNodeAtToken(currentPosition, "Expected the pragma end delimiter '>'."));
    }

    return pragma;
}

static ASTNodePtr parseIdentifierOrMacroExpansion(TokenRange &currentPosition)
{
    switch(currentPosition.peek().type)
    {
    case TokenType::Identifier: return parseIdentifierAsSymbol(currentPosition);
    case TokenType::QuasiUnquote: return parseQuasiUnquote(currentPosition);
    default: return makeParseErrorNodeAtToken(currentPosition, "Expected an identifier or a macro expansion.");
    }
}

static ASTNodePtr parseBlockClosureArgument(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    if(currentPosition.peek().type != TokenType::Colon)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a colon (:) before a block closure argument definition.");
    currentPosition.advance();

    auto node = std::make_shared<ASTBlockClosureArgumentNode> ();
    if(currentPosition.peek().type == TokenType::LeftParent)
        node->type = parseParentExpression(currentPosition);

    node->identifier = parseIdentifierOrMacroExpansion(currentPosition);
    node->setTokenRange(startPosition.until(currentPosition));
    return node;
}

static ASTNodePtr parseBlockClosureReturnType(TokenRange &currentPosition)
{
    switch(currentPosition.peek().type)
    {
    case TokenType::Identifier: return parseIdentifier(currentPosition);
    case TokenType::QuasiUnquote: return parseQuasiUnquote(currentPosition);
    case TokenType::LeftParent: return parseParentExpression(currentPosition);
    default: return makeParseErrorNodeAtToken(currentPosition, "Expected a block closure return type specification.");
    }
}

static ASTNodePtr parseBlockClosureSignature(TokenRange &currentPosition)
{
    // Parse the arguments.
    auto startPosition = currentPosition;
    auto signature = std::make_shared<ASTBlockClosureSignatureNode> ();

    while(currentPosition.peek().type == TokenType::Colon)
    {
        auto argument = parseBlockClosureArgument(currentPosition);
        signature->arguments.push_back(argument);
        if(argument->isParseErrorNode())
            break;
    }

    // Parse the return type.
    if(currentPosition.peek().type == TokenType::ColonColon)
    {
        currentPosition.advance();
        signature->returnType = parseBlockClosureReturnType(currentPosition);
    }

    // The end of the signature.
    if(currentPosition.peek().type == TokenType::BitwiseOr)
    {
        currentPosition.advance();
        signature->setTokenRange(startPosition.until(currentPosition));
        return signature;
    }

    // Restore the memento.
    currentPosition = startPosition;
    return nullptr;
}

static ASTNodePtr parseBlockExpression(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    if(currentPosition.peek().type != TokenType::LeftCurlyBracket)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a block starting with a left curly bracket ('{').");
    currentPosition.advance();

    // Create the block node.
    auto block = std::make_shared<ASTBlockNode> ();

    // Parse the block closure signature.
    block->blockClosureSignature = parseBlockClosureSignature(currentPosition);

    // Parse the pragmas.
    while(currentPosition.peek().type == TokenType::LessThan)
    {
        auto pragma = parsePragma(currentPosition);
        block->pragmas.push_back(pragma);
        if(pragma->isParseErrorNode())
            break;
    }

    // Parse the block expression list.
    block->expressionList = parseExpressionList(currentPosition);
    if(currentPosition.peek().type == TokenType::RightCurlyBracket)
    {
        currentPosition.advance();
    }
    else
    {
        if(!block->expressionList->isParseErrorNode())
            block->expressionList = makeParseErrorNodeAtToken(currentPosition, "Expected a block ending with a right curly bracket ('}').");
    }

    block->setTokenRange(startPosition.until(currentPosition));
    return block;
}

static ASTNodePtr parseDictionaryKey(TokenRange &currentPosition, bool isLiteralDictionary)
{
    // The single keyword case.
    if(currentPosition.peek().type == TokenType::Keyword)
    {
        auto symbol = std::make_shared<ASTSymbolLiteralNode> ();
        auto keywordPosition = currentPosition.until(1);
        symbol->setTokenRange(keywordPosition);

        auto keyword = currentPosition.next().text();
        assert(keyword.size() > 1);
        if(!isSingleKeyword(keyword))
            return makeParseErrorNodeAtToken(keywordPosition, "Expected a single keyword.");

        symbol->value = keyword.substr(0, keyword.size() - 1);
        return symbol;
    }

    // The expression key.
    auto key = isLiteralDictionary ? parseLiteralArrayElement(currentPosition) : parseBinaryExpression(currentPosition);

    // The separator.
    if(currentPosition.peek().type == TokenType::Colon)
    {
        currentPosition.advance();
    }
    else
    {
        if(!key->isParseErrorNode())
            key = makeParseErrorNodeAtToken(currentPosition, "Expected a colon after the key.");
    }

    return key;
}

static ASTNodePtr parseDictionaryElement(TokenRange &currentPosition, bool isLiteralDictionary)
{
    auto startPosition = currentPosition;
    auto element = std::make_shared<ASTDictionaryElementNode> ();
    element->key = parseDictionaryKey(currentPosition, isLiteralDictionary);

    switch(currentPosition.peek().type)
    {
    case TokenType::Dot:
    case TokenType::RightCurlyBracket:
        // No value.
        break;
    default:
        if(isLiteralDictionary)
            element->value = parseLiteralArrayElement(currentPosition);
        else
            element->value = parseExpression(currentPosition);
        break;
    }

    element->setTokenRange(startPosition.until(currentPosition));
    return element;
}

static ASTNodePtr parseDictionaryExpression(TokenRange &currentPosition, bool isLiteralDictionary)
{
    auto startPosition = currentPosition;
    if(currentPosition.peek().type != TokenType::DictionaryLeftBracket)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a block starting with a left curly bracket ('{').");
    currentPosition.advance();

    // Create the dictionary node.
    auto dictionary = std::make_shared<ASTMakeDictionaryNode> ();

    // Parse the dictionary elements.
    do
    {
        while(currentPosition.peek().type == TokenType::Dot)
            currentPosition.advance();

        if(currentPosition.peek().type != TokenType::RightCurlyBracket)
        {
            auto element = parseDictionaryElement(currentPosition, isLiteralDictionary);
            dictionary->elements.push_back(element);
            if(element->isParseErrorNode())
                break;
        }
    } while(currentPosition.peek().type == TokenType::Dot);

    // End parsing the dictionary.
    if(currentPosition.peek().type == TokenType::RightCurlyBracket)
    {
        currentPosition.advance();
    }
    else
    {
        if(dictionary->elements.empty() || !dictionary->elements.back()->isParseErrorNode())
            dictionary->elements.push_back(makeParseErrorNodeAtToken(currentPosition, "Expected a block ending with a right curly bracket ('}')."));
    }

    dictionary->setTokenRange(startPosition.until(currentPosition));
    return dictionary;
}

static ASTNodePtr parsePrimaryExpression(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    switch(currentPosition.peek().type)
    {
    case TokenType::LeftParent: return parseParentExpression(currentPosition);
    case TokenType::Quote: return parseQuote(currentPosition);
    case TokenType::QuasiQuote: return parseQuasiQuote(currentPosition);
    case TokenType::QuasiUnquote: return parseQuasiUnquote(currentPosition);
    case TokenType::Splice: return parseSplice(currentPosition);
    case TokenType::LeftCurlyBracket: return parseBlockExpression(currentPosition);
    case TokenType::DictionaryLeftBracket: return parseDictionaryExpression(currentPosition, false);
    case TokenType::Identifier: return parseIdentifier(currentPosition);
    default:
        return parseLiteral(currentPosition);
    }
}

static ASTNodePtrList parseCallArguments(TokenRange &currentPosition)
{
    ASTNodePtrList result;
    if(currentPosition.next().type != TokenType::LeftParent)
    {
        result.push_back(makeParseErrorNodeAtToken(currentPosition, "Expected call arguments."));
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
            result.push_back(makeParseErrorNodeAtToken(currentPosition, "Expected a right parenthesis."));
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
                messageSend->setTokenRange(startPosition.until(currentPosition));
                messageSend->receiver = receiver;
                messageSend->selector = selector;
                receiver = messageSend;
            }
            break;
        case TokenType::LeftParent:
            // Call expression
            {
                auto call = std::make_shared<ASTCallNode> ();
                call->callable = receiver;
                call->arguments = parseCallArguments(currentPosition);
                call->setTokenRange(startPosition.until(currentPosition));
                receiver = call;
            }
            break;
        case TokenType::LeftBracket:
            // Subscript expression.
            {
                // Parse the delimited index.
                currentPosition.advance();
                auto index = parseExpression(currentPosition);
                if(currentPosition.peek().type == TokenType::RightBracket)
                {
                    currentPosition.next();
                }
                else
                {
                    if(!index->isParseErrorNode())
                        index = makeParseErrorNodeAtToken(currentPosition, "Expected a right bracket ']'");
                }

                auto subscript = std::make_shared<ASTSubscriptNode> ();
                subscript->array = receiver;
                subscript->index = index;
                subscript->setTokenRange(startPosition.until(currentPosition));
                receiver = subscript;
            }
            break;
        case TokenType::QuasiUnquote:
            // Unary message with macro selector.
            {
                auto selector = parseQuasiUnquote(currentPosition);

                auto messageSend = std::make_shared<ASTMessageSendNode> ();
                messageSend->setTokenRange(startPosition.until(currentPosition));
                messageSend->receiver = receiver;
                messageSend->selector = selector;
                receiver = messageSend;
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

static ASTNodePtr parseChainReceiver(TokenRange &currentPosition)
{
    return parseBinaryExpression(currentPosition);
}

static ASTNodePtr parseChainKeywordMessage(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    if(currentPosition.peek().type != TokenType::Keyword)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a keyword chained message.");

    std::string selectorContent;
    auto chainMessage = std::make_shared<ASTMessageChainMessageNode> ();

    while(currentPosition.peek().type == TokenType::Keyword)
    {
        auto keywordPosition = currentPosition.until(1);
        auto keyword = currentPosition.next().text();
        selectorContent += keyword;

        if(!isSingleKeyword(keyword))
            chainMessage->arguments.push_back(makeParseErrorNodeAtToken(keywordPosition, "Expected a single keyword."));

        auto argument = parseBinaryExpression(currentPosition);
        chainMessage->arguments.push_back(argument);
        if(argument->isParseErrorNode())
            break;
    }

    auto selector = std::make_shared<ASTSymbolLiteralNode> ();
    selector->value = selectorContent;
    selector->setTokenRange(startPosition.until(currentPosition));

    chainMessage->selector = selector;
    chainMessage->setTokenRange(startPosition.until(currentPosition));

    return chainMessage;
}

static ASTNodePtr parseChainUnaryMessage(TokenRange &currentPosition)
{
    if(currentPosition.peek().type != TokenType::Identifier)
        return makeParseErrorNodeAtToken(currentPosition, "Expected an unary chained message.");

    auto position = currentPosition.until(1);
    auto selector = std::make_shared<ASTSymbolLiteralNode> ();
    selector->value = currentPosition.next().text();
    selector->setTokenRange(position);

    auto messageNode = std::make_shared<ASTMessageChainMessageNode> ();
    messageNode->selector = selector;
    messageNode->setTokenRange(position);
    return messageNode;
}

static ASTNodePtr parseChainedMessage(TokenRange &currentPosition)
{
    switch(currentPosition.peek().type)
    {
    case TokenType::Keyword: return parseChainKeywordMessage(currentPosition);
    case TokenType::Identifier: return parseChainUnaryMessage(currentPosition);
    default: return makeParseErrorNodeAtToken(currentPosition, "Expected a chained keyword or unary message.");
    }
}

static ASTNodePtr parseChainExpression(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    auto receiver = ASTNodePtr();

    if(currentPosition.peek().type != TokenType::Keyword)
    {
        receiver = parseChainReceiver(currentPosition);
        auto nextType = currentPosition.peek().type;
        if(nextType != TokenType::Keyword && nextType != TokenType::Semicolon)
            return receiver;
    }

    ASTNodePtr firstChainKeywordMessage;
    if(currentPosition.peek().type == TokenType::Keyword)
    {
        firstChainKeywordMessage = parseChainKeywordMessage(currentPosition);
    }


    // Not chained message case.
    if(currentPosition.peek().type != TokenType::Semicolon)
    {
        if(!firstChainKeywordMessage)
            return receiver;

        assert(firstChainKeywordMessage->isMessageChainMessageNode());
        auto &castedChain = firstChainKeywordMessage->as<ASTMessageChainMessageNode> ();

        auto messageNode = std::make_shared<ASTMessageSendNode> ();
        messageNode->setTokenRange(startPosition.until(currentPosition));
        messageNode->receiver = receiver;
        messageNode->selector = castedChain.selector;
        messageNode->arguments = castedChain.arguments;
        return messageNode;
    }

    // Message chain case.
    auto chainNode = std::make_shared<ASTMessageChainNode> ();

    if(firstChainKeywordMessage)
    {
        chainNode->receiver = receiver;
        chainNode->messages.push_back(firstChainKeywordMessage);
    }
    else
    {
        // If the receiver is a message send node, convert split it into two parts.
        if(receiver->isMessageSendNode())
        {
            auto &firstMessage = receiver->as<ASTMessageSendNode> ();
            chainNode->receiver = firstMessage.receiver;

            auto firstChain = std::make_shared<ASTMessageChainMessageNode> ();
            firstChain->tokens = receiver->tokens;
            firstChain->sourcePosition = receiver->sourcePosition;
            firstChain->selector = firstMessage.selector;
            firstChain->arguments = firstMessage.arguments;
            chainNode->messages.push_back(firstChain);
        }
        else
        {
            chainNode->receiver = receiver;
        }
    }
    while(currentPosition.peek().type == TokenType::Semicolon)
    {
        currentPosition.advance();

        auto chainedMesage = parseChainedMessage(currentPosition);
        chainNode->messages.push_back(chainedMesage);
    }

    chainNode->setTokenRange(startPosition.until(currentPosition));
    return chainNode;
}

static ASTNodePtr parseLowPrecedenceExpression(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    auto receiver = parseChainExpression(currentPosition);
    while(currentPosition.peek().type == TokenType::LowPrecedenceBinaryOperator)
    {
        auto selector = std::make_shared<ASTSymbolLiteralNode> ();
        selector->setTokenRange(currentPosition.until(1));

        auto rawSelectorValue = currentPosition.next().text();
        assert(rawSelectorValue.size() > 2);
        selector->value = rawSelectorValue.substr(2);

        auto argument = parseChainExpression(currentPosition);
        auto node = std::make_shared<ASTMessageSendNode> ();
        node->setTokenRange(startPosition.until(currentPosition));
        node->receiver = receiver;
        node->selector = selector;
        node->arguments.push_back(argument);
        receiver = node;
    }

    return receiver;
}

static ASTNodePtr parseAssignmentExpression(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    auto receiver = parseLowPrecedenceExpression(currentPosition);
    if(currentPosition.peek().type != TokenType::Assignment)
        return receiver;

    auto selector = std::make_shared<ASTSymbolLiteralNode> ();
    selector->setTokenRange(currentPosition.until(1));
    selector->value = ":=";
    currentPosition.advance();

    auto assignment = std::make_shared<ASTMessageSendNode> ();
    assignment->receiver = receiver;
    assignment->selector = selector;
    assignment->arguments.push_back(parseAssignmentExpression(currentPosition));
    assignment->setTokenRange(currentPosition.until(startPosition));
    return assignment;
}

static ASTNodePtr parseCommaExpression(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    auto first = parseAssignmentExpression(currentPosition);
    if(currentPosition.peek().type != TokenType::Comma)
        return first;

    auto node = std::make_shared<ASTMakeTupleNode> ();
    node->elements.push_back(first);
    while(currentPosition.peek().type == TokenType::Comma)
    {
        currentPosition.advance();

        auto memento = currentPosition;
        auto element = parseAssignmentExpression(currentPosition);
        if(element->isParseErrorNode())
        {
            currentPosition = memento;
            break;
        }

        node->elements.push_back(element);
    }

    node->setTokenRange(startPosition.until(currentPosition));
    return node;
}

static ASTNodePtr parseExpression(TokenRange &currentPosition)
{
    return parseCommaExpression(currentPosition);
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

ASTNodePtr parseTokenListWithLiteralArrayContent(const TokenListPtr &tokenList)
{
    auto currentPosition = TokenRange::forCollection(tokenList);
    auto expressionList = parseSourceWithLiteralArrayContent(currentPosition);
    return expressionList;
}

} // End of namespace Sysmel
} // End of namespace Frontend
} // End of namespace Sysmel
