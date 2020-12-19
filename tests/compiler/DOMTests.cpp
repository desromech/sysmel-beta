#include "sysmel/Compiler/DOM.hpp"
#include "UnitTest++/UnitTest++.h"

using namespace SysmelMoebius::Compiler;

SUITE(DOM)
{
    TEST(AtomEquality)
    {
        DOM::Value a = DOM::Nil();
        DOM::Value b = DOM::True();
        DOM::Value c = DOM::False();
        DOM::Value d = DOM::String{"Hello World"};
        DOM::Value e = DOM::Symbol{"Hello World"};

        CHECK_EQUAL(a, a);
        CHECK_EQUAL(b, b);
        CHECK_EQUAL(c, c);
        CHECK_EQUAL(d, d);
        CHECK_EQUAL(e, e);

        CHECK(a != b);
        CHECK(b != c);
        CHECK(c != d);
        CHECK(d != e);
        CHECK(e != a);

        {
            auto copy = a;
            CHECK_EQUAL(a, copy);
        }

        {
            auto copy = b;
            CHECK_EQUAL(b, copy);
        }

        {
            auto copy = c;
            CHECK_EQUAL(c, copy);
        }

        {
            auto copy = d;
            CHECK_EQUAL(d, copy);
        }

        {
            auto copy = e;
            CHECK_EQUAL(e, copy);
        }
    }

    TEST(ArrayEquality)
    {
        DOM::Value a = DOM::Array{DOM::Nil(), DOM::True(), DOM::False(), DOM::String{"Hello String"}, DOM::Symbol{"Hello Symbol"}};
        DOM::Value b = DOM::Array{DOM::Nil(), DOM::False(), DOM::True(), DOM::String{"Hello String"}, DOM::Symbol{"Hello Symbol"}};

        CHECK_EQUAL(a, a);
        CHECK_EQUAL(b, b);
        CHECK(a != b);

        {
            auto copy = a;
            CHECK_EQUAL(a, copy);
        }

        {
            auto copy = b;
            CHECK_EQUAL(b, copy);
        }
    }

    TEST(ParseArray)
    {
        DOM::Value expected = DOM::Array{DOM::Void(), DOM::Nil(), DOM::True(), DOM::False(), DOM::String{"Hello String"}, DOM::Symbol{"keyword"}};
        DOM::Value parsed = std::get<DOM::Value> (DOM::parseStringWithSysmelSyntax("void nil true false \"Hello String\" keyword"));
        CHECK_EQUAL(expected, parsed);
    }

    TEST(ParseNestedArray)
    {
        DOM::Value expected = DOM::Array{
            DOM::Array{DOM::Array{DOM::Integer{1}, DOM::Integer{2}}, DOM::Integer{3}},
            DOM::Array{DOM::Integer{4}, DOM::Integer{5}}
        };
        DOM::Value parsed = std::get<DOM::Value> (DOM::parseStringWithSysmelSyntax("((1 2) 3) (4 5)"));
        CHECK_EQUAL(expected, parsed);
    }

    TEST(ParseDictionary)
    {
        DOM::Value expected = DOM::Array{DOM::Dictionary{
            {DOM::Symbol{"A"}, DOM::Integer{1}},
            {DOM::Symbol{"B"}, DOM::Integer{2}},
        }};
        DOM::Value parsed = std::get<DOM::Value> (DOM::parseStringWithSysmelSyntax("#{A: 1. B: 2}"));
        CHECK_EQUAL(expected, parsed);
    }
}
