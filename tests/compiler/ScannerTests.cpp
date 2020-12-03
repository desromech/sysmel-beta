#include "sysmel/Compiler/Sysmel/Scanner.hpp"
#include "UnitTest++/UnitTest++.h"

using namespace SysmelMoebius::Compiler;
using namespace SysmelMoebius::Compiler::Sysmel;

static TokenType scanSingleTokenType(const std::string &source)
{
    auto tokenList = scanString(source);
    CHECK(tokenList.size() == 1 || (tokenList.size() == 2 && tokenList[1].type == TokenType::EndOfSource));
    return tokenList[0].type;
}

static std::string scanSingleTokenText(const std::string &source)
{
    auto tokenList = scanString(source);
    CHECK(tokenList.size() == 1 || (tokenList.size() == 2 && tokenList[1].type == TokenType::EndOfSource));
    return tokenList[0].text();
}

SUITE(Scanner)
{
    TEST(EndOfSource)
    {
        CHECK_EQUAL(TokenType::EndOfSource, scanSingleTokenType(""));
        CHECK_EQUAL(TokenType::EndOfSource, scanSingleTokenType("##"));
        CHECK_EQUAL(TokenType::EndOfSource, scanSingleTokenType("#* *#"));
        CHECK_EQUAL(TokenType::Error, scanSingleTokenType("#* "));
    }

    TEST(Idenifiers)
    {
        CHECK_EQUAL(TokenType::Identifier, scanSingleTokenType("A"));
        CHECK_EQUAL("A", scanSingleTokenText("A"));

        CHECK_EQUAL(TokenType::Identifier, scanSingleTokenType("_"));
        CHECK_EQUAL("_", scanSingleTokenText("_"));

        CHECK_EQUAL(TokenType::Identifier, scanSingleTokenType("a"));
        CHECK_EQUAL("a", scanSingleTokenText("a"));

        CHECK_EQUAL(TokenType::Identifier, scanSingleTokenType("helloWorld01234"));
        CHECK_EQUAL("helloWorld01234", scanSingleTokenText("helloWorld01234"));
    }

    TEST(Keywords)
    {
        CHECK_EQUAL(TokenType::Keyword, scanSingleTokenType("A:"));
        CHECK_EQUAL("A:", scanSingleTokenText("A:"));

        CHECK_EQUAL(TokenType::Keyword, scanSingleTokenType("_:"));
        CHECK_EQUAL("_:", scanSingleTokenText("_:"));

        CHECK_EQUAL(TokenType::Keyword, scanSingleTokenType("a:"));
        CHECK_EQUAL("a:", scanSingleTokenText("a:"));

        CHECK_EQUAL(TokenType::Keyword, scanSingleTokenType("helloWorld01234:"));
        CHECK_EQUAL("helloWorld01234:", scanSingleTokenText("helloWorld01234:"));

        CHECK_EQUAL(TokenType::Keyword, scanSingleTokenType("helloWorld01234:A:a1:_:"));
        CHECK_EQUAL("helloWorld01234:A:a1:_:", scanSingleTokenText("helloWorld01234:A:a1:_:"));
    }

    TEST(Integers)
    {
        CHECK_EQUAL(TokenType::Integer, scanSingleTokenType("0"));
        CHECK_EQUAL("0", scanSingleTokenText("0"));

        CHECK_EQUAL(TokenType::Integer, scanSingleTokenType("0123456789"));
        CHECK_EQUAL("0123456789", scanSingleTokenText("0123456789"));

        CHECK_EQUAL(TokenType::Integer, scanSingleTokenType("+0123456789"));
        CHECK_EQUAL("+0123456789", scanSingleTokenText("+0123456789"));

        CHECK_EQUAL(TokenType::Integer, scanSingleTokenType("-0123456789"));
        CHECK_EQUAL("-0123456789", scanSingleTokenText("-0123456789"));

        CHECK_EQUAL(TokenType::Integer, scanSingleTokenType("+32rHELLO012364WORLD"));
        CHECK_EQUAL("+32rHELLO012364WORLD", scanSingleTokenText("+32rHELLO012364WORLD"));
    }

    TEST(Floats)
    {
        CHECK_EQUAL(TokenType::Float, scanSingleTokenType("0.0"));
        CHECK_EQUAL("0.0", scanSingleTokenText("0.0"));

        CHECK_EQUAL(TokenType::Float, scanSingleTokenType("-0.0"));
        CHECK_EQUAL("-0.0", scanSingleTokenText("-0.0"));

        CHECK_EQUAL(TokenType::Float, scanSingleTokenType("+0.0"));
        CHECK_EQUAL("+0.0", scanSingleTokenText("+0.0"));

        CHECK_EQUAL(TokenType::Float, scanSingleTokenType("-0e-53"));
        CHECK_EQUAL("-0e-53", scanSingleTokenText("-0e-53"));

        CHECK_EQUAL(TokenType::Float, scanSingleTokenType("+0e+53"));
        CHECK_EQUAL("+0e+53", scanSingleTokenText("+0e+53"));

        CHECK_EQUAL(TokenType::Float, scanSingleTokenType("0123456789.14565"));
        CHECK_EQUAL("0123456789.14565", scanSingleTokenText("0123456789.14565"));

        CHECK_EQUAL(TokenType::Float, scanSingleTokenType("+0123456789.14565"));
        CHECK_EQUAL("+0123456789.14565", scanSingleTokenText("+0123456789.14565"));

        CHECK_EQUAL(TokenType::Float, scanSingleTokenType("-0123456789.14565"));
        CHECK_EQUAL("-0123456789.14565", scanSingleTokenText("-0123456789.14565"));

        CHECK_EQUAL(TokenType::Float, scanSingleTokenType("+0123456789.14565e+53"));
        CHECK_EQUAL("+0123456789.14565e+53", scanSingleTokenText("+0123456789.14565e+53"));

        CHECK_EQUAL(TokenType::Float, scanSingleTokenType("-0123456789.14565e-53"));
        CHECK_EQUAL("-0123456789.14565e-53", scanSingleTokenText("-0123456789.14565e-53"));
    }
}
