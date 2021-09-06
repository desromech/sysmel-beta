#include "sysmel/ObjectModel/LiteralInteger.hpp"
#include "sysmel/ObjectModel/LiteralNumber.hpp"
#include "sysmel/ObjectModel/RuntimeContext.hpp"
#include "sysmel/ObjectModel/Wrappers.hpp"
#include "UnitTest++/UnitTest++.h"

using namespace SysmelMoebius::ObjectModel;

SUITE(LiteralNumber)
{
    TEST(LiteralPositiveIntegerTestingMethods)
    {
        CHECK(LiteralInteger::makeFor(LargeInteger{0})->isLiteralNumber());
        CHECK(LiteralInteger::makeFor(LargeInteger{0})->isLiteralInteger());
        CHECK(LiteralInteger::makeFor(LargeInteger{0})->isLiteralPositiveInteger());
        CHECK(!LiteralInteger::makeFor(LargeInteger{0})->isLiteralNegativeInteger());

        CHECK(LiteralInteger::makeFor(LargeInteger{1})->isLiteralNumber());
        CHECK(LiteralInteger::makeFor(LargeInteger{1})->isLiteralInteger());
        CHECK(LiteralInteger::makeFor(LargeInteger{1})->isLiteralPositiveInteger());
        CHECK(!LiteralInteger::makeFor(LargeInteger{1})->isLiteralNegativeInteger());
    }

    TEST(LiteralNegativeIntegerTestingMethods)
    {
        CHECK(LiteralInteger::makeFor(LargeInteger{-1})->isLiteralNumber());
        CHECK(LiteralInteger::makeFor(LargeInteger{-1})->isLiteralInteger());
        CHECK(!LiteralInteger::makeFor(LargeInteger{-1})->isLiteralPositiveInteger());
        CHECK(LiteralInteger::makeFor(LargeInteger{-1})->isLiteralNegativeInteger());
    }

    TEST(Factorial)
    {
        RuntimeContext::create()->activeDuring([&](){
            CHECK_EQUAL(LargeInteger{1}, wrapValue(LargeInteger{0})->perform<LargeInteger> ("factorial"));
            CHECK_EQUAL(LargeInteger{2}, wrapValue(LargeInteger{2})->perform<LargeInteger> ("factorial"));
            CHECK_EQUAL(LargeInteger{6}, wrapValue(LargeInteger{3})->perform<LargeInteger> ("factorial"));
            CHECK_EQUAL(LargeInteger{24}, wrapValue(LargeInteger{4})->perform<LargeInteger> ("factorial"));
            CHECK_EQUAL(LargeInteger{120}, wrapValue(LargeInteger{5})->perform<LargeInteger> ("factorial"));
            CHECK_EQUAL(LargeInteger{720}, wrapValue(LargeInteger{6})->perform<LargeInteger> ("factorial"));
            CHECK_EQUAL(LargeInteger{5040}, wrapValue(LargeInteger{7})->perform<LargeInteger> ("factorial"));
            CHECK_EQUAL(LargeInteger{40320}, wrapValue(LargeInteger{8})->perform<LargeInteger> ("factorial"));
            CHECK_EQUAL(LargeInteger{362880}, wrapValue(LargeInteger{9})->perform<LargeInteger> ("factorial"));
            CHECK_EQUAL(LargeInteger{3628800}, wrapValue(LargeInteger{10})->perform<LargeInteger> ("factorial"));

            CHECK_EQUAL(LargeInteger{uint64_t(2432902008176640000)}, wrapValue(LargeInteger{20})->perform<LargeInteger> ("factorial"));
            CHECK_EQUAL(LargeInteger{"265252859812191058636308480000000"}, wrapValue(LargeInteger{30})->perform<LargeInteger> ("factorial"));
            CHECK_EQUAL(LargeInteger{"815915283247897734345611269596115894272000000000"}, wrapValue(LargeInteger{40})->perform<LargeInteger> ("factorial"));
            CHECK_EQUAL(LargeInteger{"30414093201713378043612608166064768844377641568960512000000000000"}, wrapValue(LargeInteger{50})->perform<LargeInteger> ("factorial"));
            CHECK_EQUAL(LargeInteger{"93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000"}, wrapValue(LargeInteger{100})->perform<LargeInteger> ("factorial"));
        });
    }
}
