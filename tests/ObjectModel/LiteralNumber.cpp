#include "sysmel/ObjectModel/LiteralInteger.hpp"
#include "sysmel/ObjectModel/LiteralNumber.hpp"
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
}
