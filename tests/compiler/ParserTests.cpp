#include "sysmel/Compiler/Sysmel/Parser.hpp"
#include "UnitTest++/UnitTest++.h"

using namespace SysmelMoebius::Compiler;
using namespace SysmelMoebius::Compiler::Sysmel;

static ASTNodePtr parseSingleExpression(const std::string &text)
{
    auto node = parseString(text);
    CHECK(node->isExpressionListNode());
    CHECK_EQUAL(1u, node->as<ASTExpressionListNode> ().expressions.size());
    return node->as<ASTExpressionListNode> ().expressions[0];
}

SUITE(Parser)
{
    TEST(Empty)
    {
        auto node = parseString("");
        CHECK(node->isExpressionListNode());
        CHECK(node->as<ASTExpressionListNode>().expressions.empty());
    }

    TEST(IntegerLiteral)
    {
        auto literal = parseSingleExpression("0");
        CHECK(literal->isIntegerLiteralNode());
        CHECK_EQUAL(LiteralInteger{0}, literal->as<ASTIntegerLiteralNode> ().value);

        literal = parseSingleExpression("-0");
        CHECK(literal->isIntegerLiteralNode());
        CHECK_EQUAL(LiteralInteger{0}, literal->as<ASTIntegerLiteralNode> ().value);

        literal = parseSingleExpression("42");
        CHECK(literal->isIntegerLiteralNode());
        CHECK_EQUAL(LiteralInteger{42}, literal->as<ASTIntegerLiteralNode> ().value);

        literal = parseSingleExpression("-42");
        CHECK(literal->isIntegerLiteralNode());
        CHECK_EQUAL(LiteralInteger{-42}, literal->as<ASTIntegerLiteralNode> ().value);

        literal = parseSingleExpression("(-42)");
        CHECK(literal->isIntegerLiteralNode());
        CHECK_EQUAL(LiteralInteger{-42}, literal->as<ASTIntegerLiteralNode> ().value);
    }

    TEST(FloatLiteral)
    {
        auto literal = parseSingleExpression("0.0");
        CHECK(literal->isFloatLiteralNode());
        CHECK_EQUAL(0.0, literal->as<ASTFloatLiteralNode> ().value);

        literal = parseSingleExpression("42.5");
        CHECK(literal->isFloatLiteralNode());
        CHECK_EQUAL(42.5, literal->as<ASTFloatLiteralNode> ().value);

        literal = parseSingleExpression("+42.5e2");
        CHECK(literal->isFloatLiteralNode());
        CHECK_EQUAL(+42.5e2, literal->as<ASTFloatLiteralNode> ().value);

        literal = parseSingleExpression("-1.5e-6");
        CHECK(literal->isFloatLiteralNode());
        CHECK_EQUAL(-1.5e-6, literal->as<ASTFloatLiteralNode> ().value);

        literal = parseSingleExpression("+42.5e+2");
        CHECK(literal->isFloatLiteralNode());
        CHECK_EQUAL(+42.5e+2, literal->as<ASTFloatLiteralNode> ().value);
    }

    TEST(IdentifierReference)
    {
        auto node = parseSingleExpression("hello");
        CHECK(node->isIdentifierReferenceNode());
        CHECK_EQUAL("hello", node->as<ASTIdentifierReferenceNode> ().identifier);

        node = parseSingleExpression("_helloWorld12345");
        CHECK(node->isIdentifierReferenceNode());
        CHECK_EQUAL("_helloWorld12345", node->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(UnaryMinus)
    {
        auto node = parseSingleExpression("-a");
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isSymbolLiteralNode());
        CHECK_EQUAL("pre--", node->as<ASTMessageSendNode> ().selector->as<ASTSymbolLiteralNode> ().value);

        CHECK(node->as<ASTMessageSendNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTMessageSendNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);
    }


    TEST(UnaryPlus)
    {
        auto node = parseSingleExpression("+a");
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isSymbolLiteralNode());
        CHECK_EQUAL("pre-+", node->as<ASTMessageSendNode> ().selector->as<ASTSymbolLiteralNode> ().value);

        CHECK(node->as<ASTMessageSendNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTMessageSendNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(LogicalNot)
    {
        auto node = parseSingleExpression("!a");
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isSymbolLiteralNode());
        CHECK_EQUAL("pre-!", node->as<ASTMessageSendNode> ().selector->as<ASTSymbolLiteralNode> ().value);

        CHECK(node->as<ASTMessageSendNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTMessageSendNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(BitwiseNot)
    {
        auto node = parseSingleExpression("~a");
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isSymbolLiteralNode());
        CHECK_EQUAL("pre-~", node->as<ASTMessageSendNode> ().selector->as<ASTSymbolLiteralNode> ().value);

        CHECK(node->as<ASTMessageSendNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTMessageSendNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);
    }
}
