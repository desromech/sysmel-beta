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

    TEST(LiteralString)
    {
        auto literal = parseSingleExpression("\"\"");
        CHECK(literal->isStringLiteralNode());
        CHECK_EQUAL("", literal->as<ASTStringLiteralNode> ().value);

        literal = parseSingleExpression("\"hello\"");
        CHECK(literal->isStringLiteralNode());
        CHECK_EQUAL("hello", literal->as<ASTStringLiteralNode> ().value);

        literal = parseSingleExpression("\"Hello World\\r\\n\"");
        CHECK(literal->isStringLiteralNode());
        CHECK_EQUAL("Hello World\r\n", literal->as<ASTStringLiteralNode> ().value);
    }

    TEST(LiteralCharacter)
    {
        auto literal = parseSingleExpression("'a'");
        CHECK(literal->isCharacterLiteralNode());
        CHECK_EQUAL(uint32_t('a'), literal->as<ASTCharacterLiteralNode> ().value);

        literal = parseSingleExpression("'\\n'");
        CHECK(literal->isCharacterLiteralNode());
        CHECK_EQUAL(uint32_t('\n'), literal->as<ASTCharacterLiteralNode> ().value);

        literal = parseSingleExpression("'\\r'");
        CHECK(literal->isCharacterLiteralNode());
        CHECK_EQUAL(uint32_t('\r'), literal->as<ASTCharacterLiteralNode> ().value);

        literal = parseSingleExpression("'\\t'");
        CHECK(literal->isCharacterLiteralNode());
        CHECK_EQUAL(uint32_t('\t'), literal->as<ASTCharacterLiteralNode> ().value);
    }

    TEST(LiteralSymbol)
    {
        auto literal = parseSingleExpression("#a");
        CHECK(literal->isSymbolLiteralNode());
        CHECK_EQUAL("a", literal->as<ASTSymbolLiteralNode> ().value);

        literal = parseSingleExpression("#test:");
        CHECK(literal->isSymbolLiteralNode());
        CHECK_EQUAL("test:", literal->as<ASTSymbolLiteralNode> ().value);

        literal = parseSingleExpression("#<");
        CHECK(literal->isSymbolLiteralNode());
        CHECK_EQUAL("<", literal->as<ASTSymbolLiteralNode> ().value);
    }

    TEST(LiteralSymbolString)
    {
        auto literal = parseSingleExpression("#\"\"");
        CHECK(literal->isStringLiteralNode());
        CHECK_EQUAL("", literal->as<ASTSymbolLiteralNode> ().value);

        literal = parseSingleExpression("#\"hello\"");
        CHECK(literal->isStringLiteralNode());
        CHECK_EQUAL("hello", literal->as<ASTSymbolLiteralNode> ().value);

        literal = parseSingleExpression("#\"Hello World\\r\\n\"");
        CHECK(literal->isStringLiteralNode());
        CHECK_EQUAL("Hello World\r\n", literal->as<ASTSymbolLiteralNode> ().value);
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

    TEST(UnaryMessage)
    {
        auto node = parseSingleExpression("a negated");
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isSymbolLiteralNode());
        CHECK_EQUAL("negated", node->as<ASTMessageSendNode> ().selector->as<ASTSymbolLiteralNode> ().value);

        CHECK(node->as<ASTMessageSendNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTMessageSendNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);
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

    TEST(BinaryOperation)
    {
        auto node = parseSingleExpression("a+b");
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isSymbolLiteralNode());
        CHECK_EQUAL("+", node->as<ASTMessageSendNode> ().selector->as<ASTSymbolLiteralNode> ().value);

        CHECK(node->as<ASTMessageSendNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTMessageSendNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK(node->as<ASTMessageSendNode> ().arguments[0]->isIdentifierReferenceNode());
        CHECK_EQUAL("b", node->as<ASTMessageSendNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(BinaryOperation2)
    {
        auto node = parseSingleExpression("a+b*c");
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isSymbolLiteralNode());
        CHECK_EQUAL("+", node->as<ASTMessageSendNode> ().selector->as<ASTSymbolLiteralNode> ().value);

        CHECK(node->as<ASTMessageSendNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTMessageSendNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);

        node = node->as<ASTMessageSendNode> ().arguments[0];
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isSymbolLiteralNode());
        CHECK_EQUAL("*", node->as<ASTMessageSendNode> ().selector->as<ASTSymbolLiteralNode> ().value);

        CHECK(node->as<ASTMessageSendNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("b", node->as<ASTMessageSendNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK(node->as<ASTMessageSendNode> ().arguments[0]->isIdentifierReferenceNode());
        CHECK_EQUAL("c", node->as<ASTMessageSendNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(LowPrecedenceBinaryOperation)
    {
        auto node = parseSingleExpression("a ::+ b");
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isSymbolLiteralNode());
        CHECK_EQUAL("+", node->as<ASTMessageSendNode> ().selector->as<ASTSymbolLiteralNode> ().value);

        CHECK(node->as<ASTMessageSendNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTMessageSendNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK(node->as<ASTMessageSendNode> ().arguments[0]->isIdentifierReferenceNode());
        CHECK_EQUAL("b", node->as<ASTMessageSendNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(LowPrecedenceBinaryOperation2)
    {
        auto node = parseSingleExpression("a ::+ b ::* c");
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isSymbolLiteralNode());
        CHECK_EQUAL("*", node->as<ASTMessageSendNode> ().selector->as<ASTSymbolLiteralNode> ().value);

        CHECK(node->as<ASTMessageSendNode> ().arguments[0]->isIdentifierReferenceNode());
        CHECK_EQUAL("c", node->as<ASTMessageSendNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);

        node = node->as<ASTMessageSendNode> ().receiver;
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isSymbolLiteralNode());
        CHECK_EQUAL("+", node->as<ASTMessageSendNode> ().selector->as<ASTSymbolLiteralNode> ().value);

        CHECK(node->as<ASTMessageSendNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTMessageSendNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK(node->as<ASTMessageSendNode> ().arguments[0]->isIdentifierReferenceNode());
        CHECK_EQUAL("b", node->as<ASTMessageSendNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(KeywordMessage)
    {
        auto node = parseSingleExpression("a computeWith: b");
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isSymbolLiteralNode());
        CHECK_EQUAL("computeWith:", node->as<ASTMessageSendNode> ().selector->as<ASTSymbolLiteralNode> ().value);

        CHECK(node->as<ASTMessageSendNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTMessageSendNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK(node->as<ASTMessageSendNode> ().arguments[0]->isIdentifierReferenceNode());
        CHECK_EQUAL("b", node->as<ASTMessageSendNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(KeywordMessageNoReceiver)
    {
        auto node = parseSingleExpression("computeWith: b");
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isSymbolLiteralNode());
        CHECK_EQUAL("computeWith:", node->as<ASTMessageSendNode> ().selector->as<ASTSymbolLiteralNode> ().value);

        CHECK_EQUAL(nullptr, node->as<ASTMessageSendNode> ().receiver);

        CHECK(node->as<ASTMessageSendNode> ().arguments[0]->isIdentifierReferenceNode());
        CHECK_EQUAL("b", node->as<ASTMessageSendNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(UnaryChainMessage)
    {
        auto node = parseSingleExpression("a hello; computeWith: c; yourself");
        CHECK(node->isMessageChainNode());
        CHECK(node->as<ASTMessageChainNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTMessageChainNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK_EQUAL(3u, node->as<ASTMessageChainNode> ().messages.size());

        {
            auto message = node->as<ASTMessageChainNode> ().messages[0];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("hello", message->as<ASTMessageChainMessageNode> ().selector->as<ASTSymbolLiteralNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments.empty());
        }

        {
            auto message = node->as<ASTMessageChainNode> ().messages[1];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("computeWith:", message->as<ASTMessageChainMessageNode> ().selector->as<ASTSymbolLiteralNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments[0]->isIdentifierReferenceNode());
            CHECK_EQUAL("c", message->as<ASTMessageChainMessageNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
        }

        {
            auto message = node->as<ASTMessageChainNode> ().messages[2];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("yourself", message->as<ASTMessageChainMessageNode> ().selector->as<ASTSymbolLiteralNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments.empty());
        }
    }

    TEST(BinaryChainMessage)
    {
        auto node = parseSingleExpression("a + b; computeWith: c; yourself");
        CHECK(node->isMessageChainNode());
        CHECK(node->as<ASTMessageChainNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTMessageChainNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK_EQUAL(3u, node->as<ASTMessageChainNode> ().messages.size());

        {
            auto message = node->as<ASTMessageChainNode> ().messages[0];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("+", message->as<ASTMessageChainMessageNode> ().selector->as<ASTSymbolLiteralNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments[0]->isIdentifierReferenceNode());
            CHECK_EQUAL("b", message->as<ASTMessageChainMessageNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
        }

        {
            auto message = node->as<ASTMessageChainNode> ().messages[1];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("computeWith:", message->as<ASTMessageChainMessageNode> ().selector->as<ASTSymbolLiteralNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments[0]->isIdentifierReferenceNode());
            CHECK_EQUAL("c", message->as<ASTMessageChainMessageNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
        }

        {
            auto message = node->as<ASTMessageChainNode> ().messages[2];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("yourself", message->as<ASTMessageChainMessageNode> ().selector->as<ASTSymbolLiteralNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments.empty());
        }
    }

    TEST(KeywordChainMessage)
    {
        auto node = parseSingleExpression("a computeWith: b; computeWith: c; yourself");
        CHECK(node->isMessageChainNode());
        CHECK(node->as<ASTMessageChainNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTMessageChainNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK_EQUAL(3u, node->as<ASTMessageChainNode> ().messages.size());

        {
            auto message = node->as<ASTMessageChainNode> ().messages[0];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("computeWith:", message->as<ASTMessageChainMessageNode> ().selector->as<ASTSymbolLiteralNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments[0]->isIdentifierReferenceNode());
            CHECK_EQUAL("b", message->as<ASTMessageChainMessageNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
        }

        {
            auto message = node->as<ASTMessageChainNode> ().messages[1];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("computeWith:", message->as<ASTMessageChainMessageNode> ().selector->as<ASTSymbolLiteralNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments[0]->isIdentifierReferenceNode());
            CHECK_EQUAL("c", message->as<ASTMessageChainMessageNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
        }

        {
            auto message = node->as<ASTMessageChainNode> ().messages[2];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("yourself", message->as<ASTMessageChainMessageNode> ().selector->as<ASTSymbolLiteralNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments.empty());
        }
    }

    TEST(KeywordChainMessageNoReceiver)
    {
        auto node = parseSingleExpression("computeWith: b; computeWith: c; yourself");
        CHECK(node->isMessageChainNode());
        CHECK_EQUAL(nullptr, node->as<ASTMessageChainNode> ().receiver);

        CHECK_EQUAL(3u, node->as<ASTMessageChainNode> ().messages.size());

        {
            auto message = node->as<ASTMessageChainNode> ().messages[0];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("computeWith:", message->as<ASTMessageChainMessageNode> ().selector->as<ASTSymbolLiteralNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments[0]->isIdentifierReferenceNode());
            CHECK_EQUAL("b", message->as<ASTMessageChainMessageNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
        }

        {
            auto message = node->as<ASTMessageChainNode> ().messages[1];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("computeWith:", message->as<ASTMessageChainMessageNode> ().selector->as<ASTSymbolLiteralNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments[0]->isIdentifierReferenceNode());
            CHECK_EQUAL("c", message->as<ASTMessageChainMessageNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
        }

        {
            auto message = node->as<ASTMessageChainNode> ().messages[2];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("yourself", message->as<ASTMessageChainMessageNode> ().selector->as<ASTSymbolLiteralNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments.empty());
        }
    }

    TEST(AssignmentOperation)
    {
        auto node = parseSingleExpression("a:=b");
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isSymbolLiteralNode());
        CHECK_EQUAL(":=", node->as<ASTMessageSendNode> ().selector->as<ASTSymbolLiteralNode> ().value);

        CHECK(node->as<ASTMessageSendNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTMessageSendNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK(node->as<ASTMessageSendNode> ().arguments[0]->isIdentifierReferenceNode());
        CHECK_EQUAL("b", node->as<ASTMessageSendNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(AssignmentOperation2)
    {
        auto node = parseSingleExpression("a:=b:=c");
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isSymbolLiteralNode());
        CHECK_EQUAL(":=", node->as<ASTMessageSendNode> ().selector->as<ASTSymbolLiteralNode> ().value);

        CHECK(node->as<ASTMessageSendNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTMessageSendNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);

        node = node->as<ASTMessageSendNode> ().arguments[0];
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isSymbolLiteralNode());
        CHECK_EQUAL(":=", node->as<ASTMessageSendNode> ().selector->as<ASTSymbolLiteralNode> ().value);

        CHECK(node->as<ASTMessageSendNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("b", node->as<ASTMessageSendNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK(node->as<ASTMessageSendNode> ().arguments[0]->isIdentifierReferenceNode());
        CHECK_EQUAL("c", node->as<ASTMessageSendNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(EmptyTuple)
    {
        auto node = parseSingleExpression("()");
        CHECK(node->isMakeTupleNode());
        CHECK(node->as<ASTMakeTupleNode> ().elements.empty());
    }

    TEST(MakeTuple)
    {
        auto node = parseSingleExpression("a,");
        CHECK(node->isMakeTupleNode());
        auto &elements = node->as<ASTMakeTupleNode> ().elements;
        CHECK_EQUAL(1u, elements.size());

        auto element = elements[0];
        CHECK(element->isIdentifierReferenceNode());
        CHECK_EQUAL("a", element->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(MakeTuple2)
    {
        auto node = parseSingleExpression("a,b");
        CHECK(node->isMakeTupleNode());
        auto &elements = node->as<ASTMakeTupleNode> ().elements;
        CHECK_EQUAL(2u, elements.size());

        auto element = elements[0];
        CHECK(element->isIdentifierReferenceNode());
        CHECK_EQUAL("a", element->as<ASTIdentifierReferenceNode> ().identifier);

        element = elements[1];
        CHECK(element->isIdentifierReferenceNode());
        CHECK_EQUAL("b", element->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(MakeTuple3)
    {
        auto node = parseSingleExpression("a,b,");
        CHECK(node->isMakeTupleNode());
        auto &elements = node->as<ASTMakeTupleNode> ().elements;
        CHECK_EQUAL(2u, elements.size());

        auto element = elements[0];
        CHECK(element->isIdentifierReferenceNode());
        CHECK_EQUAL("a", element->as<ASTIdentifierReferenceNode> ().identifier);

        element = elements[1];
        CHECK(element->isIdentifierReferenceNode());
        CHECK_EQUAL("b", element->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(EmptyLiteralArray)
    {
        auto node = parseSingleExpression("#()");
        CHECK(node->isLiteralArrayNode());
        CHECK(node->as<ASTLiteralArrayNode> ().elements.empty());
    }

    TEST(LiteralArray)
    {
        auto node = parseSingleExpression("#(a)");
        CHECK(node->isLiteralArrayNode());
        auto &elements = node->as<ASTLiteralArrayNode> ().elements;
        CHECK_EQUAL(1u, elements.size());

        auto element = elements[0];
        CHECK(element->isIdentifierReferenceNode());
        CHECK_EQUAL("a", element->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(LiteralArray2)
    {
        auto node = parseSingleExpression("#(a b)");
        CHECK(node->isLiteralArrayNode());
        auto &elements = node->as<ASTLiteralArrayNode> ().elements;
        CHECK_EQUAL(2u, elements.size());

        auto element = elements[0];
        CHECK(element->isIdentifierReferenceNode());
        CHECK_EQUAL("a", element->as<ASTIdentifierReferenceNode> ().identifier);

        element = elements[1];
        CHECK(element->isIdentifierReferenceNode());
        CHECK_EQUAL("b", element->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(LiteralArray3)
    {
        auto node = parseSingleExpression("#(hello:World: #b)");
        CHECK(node->isLiteralArrayNode());
        auto &elements = node->as<ASTLiteralArrayNode> ().elements;
        CHECK_EQUAL(2u, elements.size());

        auto element = elements[0];
        CHECK(element->isSymbolLiteralNode());
        CHECK_EQUAL("hello:World:", element->as<ASTSymbolLiteralNode> ().value);

        element = elements[1];
        CHECK(element->isSymbolLiteralNode());
        CHECK_EQUAL("b", element->as<ASTSymbolLiteralNode> ().value);
    }

    TEST(LiteralArray4)
    {
        std::vector operators = {"*", "/", "//", "%", "+", "-", "<<", ">>",
            "<=", ">=", "<", ">", "=", "==", "~=", "~~", "&", "|", "&&",
            "||", "==>", "-->"};
        auto node = parseSingleExpression("#(* / // % + - << >> <= >= < > = == ~= ~~ & | && || ==> --> )");
        CHECK(node->isLiteralArrayNode());
        auto &elements = node->as<ASTLiteralArrayNode> ().elements;
        CHECK_EQUAL(operators.size(), elements.size());

        for(size_t i = 0; i < operators.size(); ++i)
        {
            auto element = elements[i];
            CHECK(element->isSymbolLiteralNode());
            CHECK_EQUAL(operators[i], element->as<ASTSymbolLiteralNode> ().value);
        }
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

    TEST(CallExpression)
    {
        auto node = parseSingleExpression("a()");
        CHECK(node->isCallNode());

        CHECK(node->as<ASTCallNode> ().callable->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTCallNode> ().callable->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK(node->as<ASTCallNode> ().arguments.empty());
    }

    TEST(CallExpression2)
    {
        auto node = parseSingleExpression("a(42)");
        CHECK(node->isCallNode());

        CHECK(node->as<ASTCallNode> ().callable->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTCallNode> ().callable->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK_EQUAL(1u, node->as<ASTCallNode> ().arguments.size());

        CHECK(node->as<ASTCallNode> ().arguments[0]->isIntegerLiteralNode());
        CHECK_EQUAL(LiteralInteger{42u}, node->as<ASTCallNode> ().arguments[0]->as<ASTIntegerLiteralNode> ().value);
    }

    TEST(CallExpression3)
    {
        auto node = parseSingleExpression("a(42, b)");
        CHECK(node->isCallNode());

        CHECK(node->as<ASTCallNode> ().callable->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTCallNode> ().callable->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK_EQUAL(2u, node->as<ASTCallNode> ().arguments.size());

        CHECK(node->as<ASTCallNode> ().arguments[0]->isIntegerLiteralNode());
        CHECK_EQUAL(LiteralInteger{42}, node->as<ASTCallNode> ().arguments[0]->as<ASTIntegerLiteralNode> ().value);

        CHECK(node->as<ASTCallNode> ().arguments[1]->isIdentifierReferenceNode());
        CHECK_EQUAL("b", node->as<ASTCallNode> ().arguments[1]->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(SubscriptExpression)
    {
        auto node = parseSingleExpression("a[42]");
        CHECK(node->isSubscriptNode());

        CHECK(node->as<ASTSubscriptNode> ().array->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTSubscriptNode> ().array->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK(node->as<ASTSubscriptNode> ().index->isIntegerLiteralNode());
        CHECK_EQUAL(LiteralInteger{42}, node->as<ASTSubscriptNode> ().index->as<ASTIntegerLiteralNode> ().value);
    }

    TEST(QuoteNode)
    {
        auto node = parseSingleExpression("`'a");
        CHECK(node->isQuoteNode());

        CHECK(node->as<ASTQuoteNode> ().quoted->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTQuoteNode> ().quoted->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(QuasiQuoteNode)
    {
        auto node = parseSingleExpression("``a");
        CHECK(node->isQuasiQuoteNode());

        CHECK(node->as<ASTQuasiQuoteNode> ().quoted->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTQuasiQuoteNode> ().quoted->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(QuasiUnquoteNode)
    {
        auto node = parseSingleExpression("`,a");
        CHECK(node->isQuasiUnquoteNode());

        CHECK(node->as<ASTQuasiUnquoteNode> ().expression->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTQuasiUnquoteNode> ().expression->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(SpliceNode)
    {
        auto node = parseSingleExpression("`@a");
        CHECK(node->isSpliceNode());

        CHECK(node->as<ASTSpliceNode> ().expression->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTSpliceNode> ().expression->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(UnaryMessageWithMacroSelector)
    {
        auto node = parseSingleExpression("a `,b");
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isQuasiUnquoteNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->as<ASTQuasiUnquoteNode> ().expression->isIdentifierReferenceNode());
        CHECK_EQUAL("b", node->as<ASTMessageSendNode> ().selector->as<ASTQuasiUnquoteNode> ().expression->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(BlockNode)
    {
        auto node = parseSingleExpression("{}");
        CHECK(node->isBlockNode());
        CHECK(node->as<ASTBlockNode> ().pragmas.empty());
        CHECK(node->as<ASTBlockNode> ().expressionList->isExpressionListNode());
        CHECK(node->as<ASTBlockNode> ().expressionList->as<ASTExpressionListNode> ().expressions.empty());
    }

    TEST(BlockNode2)
    {
        auto node = parseSingleExpression("{1}");
        CHECK(node->isBlockNode());
        CHECK(node->as<ASTBlockNode> ().pragmas.empty());
        CHECK(node->as<ASTBlockNode> ().expressionList->isExpressionListNode());
        CHECK_EQUAL(1u, node->as<ASTBlockNode> ().expressionList->as<ASTExpressionListNode> ().expressions.size());

        CHECK(node->as<ASTBlockNode> ().expressionList->as<ASTExpressionListNode> ().expressions[0]->isIntegerLiteralNode());
    }

    TEST(BlockClosureNode)
    {
        auto node = parseSingleExpression("{| }");
        CHECK(node->isBlockNode());
        CHECK(node->as<ASTBlockNode> ().blockClosureSignature->isBlockClosureSignatureNode());
        auto &signature = node->as<ASTBlockNode> ().blockClosureSignature->as<ASTBlockClosureSignatureNode> ();
        CHECK(signature.arguments.empty());
        CHECK_EQUAL(nullptr, signature.returnType);

        CHECK(node->as<ASTBlockNode> ().pragmas.empty());
        CHECK(node->as<ASTBlockNode> ().expressionList->isExpressionListNode());
        CHECK(node->as<ASTBlockNode> ().expressionList->as<ASTExpressionListNode> ().expressions.empty());
    }

    TEST(BlockClosureNode2)
    {
        auto node = parseSingleExpression("{:: Void | }");
        CHECK(node->isBlockNode());
        CHECK(node->as<ASTBlockNode> ().blockClosureSignature->isBlockClosureSignatureNode());

        auto &signature = node->as<ASTBlockNode> ().blockClosureSignature->as<ASTBlockClosureSignatureNode> ();
        CHECK(signature.arguments.empty());
        CHECK(signature.returnType->isIdentifierReferenceNode());
        CHECK_EQUAL("Void", signature.returnType->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK(node->as<ASTBlockNode> ().pragmas.empty());
        CHECK(node->as<ASTBlockNode> ().expressionList->isExpressionListNode());
        CHECK(node->as<ASTBlockNode> ().expressionList->as<ASTExpressionListNode> ().expressions.empty());
    }

    TEST(BlockClosureNode3)
    {
        auto node = parseSingleExpression("{:a | }");
        CHECK(node->isBlockNode());
        CHECK(node->as<ASTBlockNode> ().blockClosureSignature->isBlockClosureSignatureNode());

        auto &signature = node->as<ASTBlockNode> ().blockClosureSignature->as<ASTBlockClosureSignatureNode> ();
        CHECK_EQUAL(nullptr, signature.returnType);
        CHECK_EQUAL(1u, signature.arguments.size());

        CHECK(signature.arguments[0]->isBlockClosureArgumentNode());

        auto argument = &signature.arguments[0]->as<ASTBlockClosureArgumentNode> ();
        CHECK(argument->identifier->isSymbolLiteralNode());
        CHECK_EQUAL("a", argument->identifier->as<ASTSymbolLiteralNode> ().value);
        CHECK_EQUAL(nullptr, argument->type);

        CHECK(node->as<ASTBlockNode> ().pragmas.empty());
        CHECK(node->as<ASTBlockNode> ().expressionList->isExpressionListNode());
        CHECK(node->as<ASTBlockNode> ().expressionList->as<ASTExpressionListNode> ().expressions.empty());
    }

    TEST(BlockClosureNode4)
    {
        auto node = parseSingleExpression("{:a :(Int32)b :c | }");
        CHECK(node->isBlockNode());
        CHECK(node->as<ASTBlockNode> ().blockClosureSignature->isBlockClosureSignatureNode());

        auto &signature = node->as<ASTBlockNode> ().blockClosureSignature->as<ASTBlockClosureSignatureNode> ();
        CHECK_EQUAL(nullptr, signature.returnType);
        CHECK_EQUAL(3u, signature.arguments.size());

        CHECK(signature.arguments[0]->isBlockClosureArgumentNode());

        auto argument = &signature.arguments[0]->as<ASTBlockClosureArgumentNode> ();
        CHECK(argument->identifier->isSymbolLiteralNode());
        CHECK_EQUAL("a", argument->identifier->as<ASTSymbolLiteralNode> ().value);
        CHECK_EQUAL(nullptr, argument->type);

        argument = &signature.arguments[1]->as<ASTBlockClosureArgumentNode> ();
        CHECK(argument->type->isIdentifierReferenceNode());
        CHECK_EQUAL("Int32", argument->type->as<ASTIdentifierReferenceNode> ().identifier);
        CHECK(argument->identifier->isSymbolLiteralNode());
        CHECK_EQUAL("b", argument->identifier->as<ASTSymbolLiteralNode> ().value);

        argument = &signature.arguments[2]->as<ASTBlockClosureArgumentNode> ();
        CHECK(argument->identifier->isSymbolLiteralNode());
        CHECK_EQUAL("c", argument->identifier->as<ASTSymbolLiteralNode> ().value);
        CHECK_EQUAL(nullptr, argument->type);

        CHECK(node->as<ASTBlockNode> ().pragmas.empty());
        CHECK(node->as<ASTBlockNode> ().expressionList->isExpressionListNode());
        CHECK(node->as<ASTBlockNode> ().expressionList->as<ASTExpressionListNode> ().expressions.empty());
    }

    TEST(BlockClosureNode5)
    {
        auto node = parseSingleExpression("{:(Float32)a :(Int32)b :: Float32 | a + b}");
        CHECK(node->isBlockNode());
        CHECK(node->as<ASTBlockNode> ().blockClosureSignature->isBlockClosureSignatureNode());

        auto &signature = node->as<ASTBlockNode> ().blockClosureSignature->as<ASTBlockClosureSignatureNode> ();
        CHECK(signature.returnType->isIdentifierReferenceNode());
        CHECK_EQUAL("Float32", signature.returnType->as<ASTIdentifierReferenceNode> ().identifier);
        CHECK_EQUAL(2u, signature.arguments.size());

        CHECK(signature.arguments[0]->isBlockClosureArgumentNode());

        auto argument = &signature.arguments[0]->as<ASTBlockClosureArgumentNode> ();
        CHECK(argument->type->isIdentifierReferenceNode());
        CHECK_EQUAL("Float32", argument->type->as<ASTIdentifierReferenceNode> ().identifier);
        CHECK(argument->identifier->isSymbolLiteralNode());
        CHECK_EQUAL("a", argument->identifier->as<ASTSymbolLiteralNode> ().value);

        argument = &signature.arguments[1]->as<ASTBlockClosureArgumentNode> ();
        CHECK(argument->type->isIdentifierReferenceNode());
        CHECK_EQUAL("Int32", argument->type->as<ASTIdentifierReferenceNode> ().identifier);
        CHECK(argument->identifier->isSymbolLiteralNode());
        CHECK_EQUAL("b", argument->identifier->as<ASTSymbolLiteralNode> ().value);

        CHECK(node->as<ASTBlockNode> ().pragmas.empty());
        CHECK(node->as<ASTBlockNode> ().expressionList->isExpressionListNode());
        CHECK_EQUAL(1u, node->as<ASTBlockNode> ().expressionList->as<ASTExpressionListNode> ().expressions.size());

        auto expression = node->as<ASTBlockNode> ().expressionList->as<ASTExpressionListNode> ().expressions[0];
        CHECK(expression->isMessageSendNode());
        CHECK(expression->as<ASTMessageSendNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL(1u, expression->as<ASTMessageSendNode> ().arguments.size());
        CHECK(expression->as<ASTMessageSendNode> ().arguments[0]->isIdentifierReferenceNode());
    }

    TEST(MakeEmptyDictionary)
    {
        auto node = parseSingleExpression("#{}");
        CHECK(node->isMakeDictionaryNode());
        CHECK(node->as<ASTMakeDictionaryNode> ().elements.empty());
    }

    TEST(MakeEmptyDictionary2)
    {
        auto node = parseSingleExpression("#{.}");
        CHECK(node->isMakeDictionaryNode());
        CHECK(node->as<ASTMakeDictionaryNode> ().elements.empty());
    }

    TEST(MakeEmptyDictionary3)
    {
        auto node = parseSingleExpression("#{..}");
        CHECK(node->isMakeDictionaryNode());
        CHECK(node->as<ASTMakeDictionaryNode> ().elements.empty());
    }

    TEST(MakeDictionary)
    {
        auto node = parseSingleExpression("#{A: 1}");
        CHECK(node->isMakeDictionaryNode());
        CHECK_EQUAL(1u, node->as<ASTMakeDictionaryNode> ().elements.size());

        auto element = node->as<ASTMakeDictionaryNode> ().elements[0];
        CHECK(element->isDictionaryElementNode());
        CHECK(element->as<ASTDictionaryElementNode> ().key->isSymbolLiteralNode());
        CHECK_EQUAL("A", element->as<ASTDictionaryElementNode> ().key->as<ASTSymbolLiteralNode> ().value);

        CHECK(element->as<ASTDictionaryElementNode> ().value->isIntegerLiteralNode());
        CHECK_EQUAL(LiteralInteger{1u}, element->as<ASTDictionaryElementNode> ().value->as<ASTIntegerLiteralNode> ().value);
    }

    TEST(MakeDictionary2)
    {
        auto node = parseSingleExpression("#{A:}");
        CHECK(node->isMakeDictionaryNode());
        CHECK_EQUAL(1u, node->as<ASTMakeDictionaryNode> ().elements.size());

        auto element = node->as<ASTMakeDictionaryNode> ().elements[0];
        CHECK(element->isDictionaryElementNode());
        CHECK(element->as<ASTDictionaryElementNode> ().key->isSymbolLiteralNode());
        CHECK_EQUAL("A", element->as<ASTDictionaryElementNode> ().key->as<ASTSymbolLiteralNode> ().value);

        CHECK_EQUAL(nullptr, element->as<ASTDictionaryElementNode> ().value);
    }

    TEST(MakeDictionary3)
    {
        auto node = parseSingleExpression("#{A: 1 .. B: 2}");
        CHECK(node->isMakeDictionaryNode());
        CHECK_EQUAL(2u, node->as<ASTMakeDictionaryNode> ().elements.size());

        auto element = node->as<ASTMakeDictionaryNode> ().elements[0];
        CHECK(element->isDictionaryElementNode());
        CHECK(element->as<ASTDictionaryElementNode> ().key->isSymbolLiteralNode());
        CHECK_EQUAL("A", element->as<ASTDictionaryElementNode> ().key->as<ASTSymbolLiteralNode> ().value);

        CHECK(element->as<ASTDictionaryElementNode> ().value->isIntegerLiteralNode());
        CHECK_EQUAL(LiteralInteger{1u}, element->as<ASTDictionaryElementNode> ().value->as<ASTIntegerLiteralNode> ().value);

        element = node->as<ASTMakeDictionaryNode> ().elements[1];
        CHECK(element->isDictionaryElementNode());
        CHECK(element->as<ASTDictionaryElementNode> ().key->isSymbolLiteralNode());
        CHECK_EQUAL("B", element->as<ASTDictionaryElementNode> ().key->as<ASTSymbolLiteralNode> ().value);

        CHECK(element->as<ASTDictionaryElementNode> ().value->isIntegerLiteralNode());
        CHECK_EQUAL(LiteralInteger{2u}, element->as<ASTDictionaryElementNode> ().value->as<ASTIntegerLiteralNode> ().value);
    }

    TEST(MakeDictionary4)
    {
        auto node = parseSingleExpression("#{#A : 1 .. B: 2}");
        CHECK(node->isMakeDictionaryNode());
        CHECK_EQUAL(2u, node->as<ASTMakeDictionaryNode> ().elements.size());

        auto element = node->as<ASTMakeDictionaryNode> ().elements[0];
        CHECK(element->isDictionaryElementNode());
        CHECK(element->as<ASTDictionaryElementNode> ().key->isSymbolLiteralNode());
        CHECK_EQUAL("A", element->as<ASTDictionaryElementNode> ().key->as<ASTSymbolLiteralNode> ().value);

        CHECK(element->as<ASTDictionaryElementNode> ().value->isIntegerLiteralNode());
        CHECK_EQUAL(LiteralInteger{1u}, element->as<ASTDictionaryElementNode> ().value->as<ASTIntegerLiteralNode> ().value);

        element = node->as<ASTMakeDictionaryNode> ().elements[1];
        CHECK(element->isDictionaryElementNode());
        CHECK(element->as<ASTDictionaryElementNode> ().key->isSymbolLiteralNode());
        CHECK_EQUAL("B", element->as<ASTDictionaryElementNode> ().key->as<ASTSymbolLiteralNode> ().value);

        CHECK(element->as<ASTDictionaryElementNode> ().value->isIntegerLiteralNode());
        CHECK_EQUAL(LiteralInteger{2u}, element->as<ASTDictionaryElementNode> ().value->as<ASTIntegerLiteralNode> ().value);
    }

    TEST(MakeDictionary5)
    {
        auto node = parseSingleExpression("#{#A : 1 .. B:.}");
        CHECK(node->isMakeDictionaryNode());
        CHECK_EQUAL(2u, node->as<ASTMakeDictionaryNode> ().elements.size());

        auto element = node->as<ASTMakeDictionaryNode> ().elements[0];
        CHECK(element->isDictionaryElementNode());
        CHECK(element->as<ASTDictionaryElementNode> ().key->isSymbolLiteralNode());
        CHECK_EQUAL("A", element->as<ASTDictionaryElementNode> ().key->as<ASTSymbolLiteralNode> ().value);

        CHECK(element->as<ASTDictionaryElementNode> ().value->isIntegerLiteralNode());
        CHECK_EQUAL(LiteralInteger{1u}, element->as<ASTDictionaryElementNode> ().value->as<ASTIntegerLiteralNode> ().value);

        element = node->as<ASTMakeDictionaryNode> ().elements[1];
        CHECK(element->isDictionaryElementNode());
        CHECK(element->as<ASTDictionaryElementNode> ().key->isSymbolLiteralNode());
        CHECK_EQUAL("B", element->as<ASTDictionaryElementNode> ().key->as<ASTSymbolLiteralNode> ().value);

        CHECK_EQUAL(nullptr, element->as<ASTDictionaryElementNode> ().value);
    }

    TEST(MakeDictionary6)
    {
        auto node = parseSingleExpression("#{A: 1. B: . C: .}");
        CHECK(node->isMakeDictionaryNode());
        CHECK_EQUAL(3u, node->as<ASTMakeDictionaryNode> ().elements.size());

        auto element = node->as<ASTMakeDictionaryNode> ().elements[0];
        CHECK(element->isDictionaryElementNode());
        CHECK(element->as<ASTDictionaryElementNode> ().key->isSymbolLiteralNode());
        CHECK_EQUAL("A", element->as<ASTDictionaryElementNode> ().key->as<ASTSymbolLiteralNode> ().value);

        CHECK(element->as<ASTDictionaryElementNode> ().value->isIntegerLiteralNode());
        CHECK_EQUAL(LiteralInteger{1u}, element->as<ASTDictionaryElementNode> ().value->as<ASTIntegerLiteralNode> ().value);

        element = node->as<ASTMakeDictionaryNode> ().elements[1];
        CHECK(element->isDictionaryElementNode());
        CHECK(element->as<ASTDictionaryElementNode> ().key->isSymbolLiteralNode());
        CHECK_EQUAL("B", element->as<ASTDictionaryElementNode> ().key->as<ASTSymbolLiteralNode> ().value);
        CHECK_EQUAL(nullptr, element->as<ASTDictionaryElementNode> ().value);

        element = node->as<ASTMakeDictionaryNode> ().elements[2];
        CHECK(element->isDictionaryElementNode());
        CHECK(element->as<ASTDictionaryElementNode> ().key->isSymbolLiteralNode());
        CHECK_EQUAL("C", element->as<ASTDictionaryElementNode> ().key->as<ASTSymbolLiteralNode> ().value);
        CHECK_EQUAL(nullptr, element->as<ASTDictionaryElementNode> ().value);
    }

    TEST(UnaryPragma)
    {
        auto node = parseSingleExpression("{<pragma>}");
        CHECK(node->isBlockNode());
        CHECK_EQUAL(1u, node->as<ASTBlockNode> ().pragmas.size());

        auto pragma = node->as<ASTBlockNode> ().pragmas[0];
        CHECK(pragma->isPragmaNode());
        CHECK(pragma->as<ASTPragmaNode> ().selector->isSymbolLiteralNode());
        CHECK_EQUAL("pragma", pragma->as<ASTPragmaNode> ().selector->as<ASTSymbolLiteralNode> ().value);

        CHECK(pragma->as<ASTPragmaNode> ().arguments.empty());
    }

    TEST(KeywordPragma)
    {
        auto node = parseSingleExpression("{<pragma: 42>}");
        CHECK(node->isBlockNode());
        CHECK_EQUAL(1u, node->as<ASTBlockNode> ().pragmas.size());

        auto pragma = node->as<ASTBlockNode> ().pragmas[0];
        CHECK(pragma->isPragmaNode());
        CHECK(pragma->as<ASTPragmaNode> ().selector->isSymbolLiteralNode());
        CHECK_EQUAL("pragma:", pragma->as<ASTPragmaNode> ().selector->as<ASTSymbolLiteralNode> ().value);

        CHECK_EQUAL(1u, pragma->as<ASTPragmaNode> ().arguments.size());
        CHECK(pragma->as<ASTPragmaNode> ().arguments[0]->isIntegerLiteralNode());
        CHECK_EQUAL(LiteralInteger{42}, pragma->as<ASTPragmaNode> ().arguments[0]->as<ASTIntegerLiteralNode> ().value);
    }

    TEST(KeywordPragma2)
    {
        auto node = parseSingleExpression("{<pragma: 42 value: -5>}");
        CHECK(node->isBlockNode());
        CHECK_EQUAL(1u, node->as<ASTBlockNode> ().pragmas.size());

        auto pragma = node->as<ASTBlockNode> ().pragmas[0];
        CHECK(pragma->isPragmaNode());
        CHECK(pragma->as<ASTPragmaNode> ().selector->isSymbolLiteralNode());
        CHECK_EQUAL("pragma:value:", pragma->as<ASTPragmaNode> ().selector->as<ASTSymbolLiteralNode> ().value);

        CHECK_EQUAL(2u, pragma->as<ASTPragmaNode> ().arguments.size());

        CHECK(pragma->as<ASTPragmaNode> ().arguments[0]->isIntegerLiteralNode());
        CHECK_EQUAL(LiteralInteger{42}, pragma->as<ASTPragmaNode> ().arguments[0]->as<ASTIntegerLiteralNode> ().value);

        CHECK(pragma->as<ASTPragmaNode> ().arguments[1]->isIntegerLiteralNode());
        CHECK_EQUAL(LiteralInteger{-5}, pragma->as<ASTPragmaNode> ().arguments[1]->as<ASTIntegerLiteralNode> ().value);
    }

}
