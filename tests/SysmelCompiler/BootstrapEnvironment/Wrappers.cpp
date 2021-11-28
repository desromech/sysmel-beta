#include "sysmel/BootstrapEnvironment/Wrappers.hpp"
#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"
#include "UnitTest++/UnitTest++.h"

using namespace SysmelMoebius::BootstrapEnvironment;

SUITE(BootstrapModule)
{
    TEST(Integer)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            CHECK(wrapValue(0)->isLiteralPositiveInteger());
            CHECK_EQUAL(0, wrapValue(0)->unwrapAsInt32());
            CHECK_EQUAL(0, unwrapValue<int> (wrapValue(0)));

            CHECK(wrapValue(1)->isLiteralPositiveInteger());
            CHECK_EQUAL(1, wrapValue(1)->unwrapAsInt32());
            CHECK_EQUAL(1, unwrapValue<int> (wrapValue(1)));

            CHECK(wrapValue(-1)->isLiteralNegativeInteger());
            CHECK_EQUAL(-1, wrapValue(-1)->unwrapAsInt32());
            CHECK_EQUAL(-1, unwrapValue<int> (wrapValue(-1)));
        });
    }

    TEST(Float)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            CHECK(wrapValue(0.0)->isLiteralFloat());
            CHECK_EQUAL(0.0, wrapValue(0.0)->unwrapAsFloat64());
            CHECK_EQUAL(0.0, unwrapValue<double> (wrapValue(0.0)));

            CHECK(wrapValue(42.5)->isLiteralFloat());
            CHECK_EQUAL(42.5, wrapValue(42.5)->unwrapAsFloat64());
            CHECK_EQUAL(42.5, unwrapValue<double> (wrapValue(42.5)));

            CHECK(wrapValue(0.0f)->isLiteralFloat());
            CHECK_EQUAL(0.0f, wrapValue(0.0)->unwrapAsFloat32());
            CHECK_EQUAL(0.0f, unwrapValue<float> (wrapValue(0.0)));

            CHECK(wrapValue(42.5f)->isLiteralFloat());
            CHECK_EQUAL(42.5f, wrapValue(42.5f)->unwrapAsFloat32());
            CHECK_EQUAL(42.5f, unwrapValue<float> (wrapValue(42.5f)));
        });
    }

    TEST(Character)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            CHECK(wrapValue('A')->isLiteralCharacter());
            CHECK_EQUAL('A', wrapValue('A')->unwrapAsChar8());
            CHECK_EQUAL('A', unwrapValue<char> (wrapValue('A')));
        });
    }

    TEST(String)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            CHECK(wrapValue("Hello World")->isLiteralString());
            CHECK_EQUAL("Hello World", wrapValue("Hello World")->unwrapAsString());
            CHECK_EQUAL("Hello World", unwrapValue<std::string> (wrapValue("Hello World")));
        });
    }
}
