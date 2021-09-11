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

    TEST(Negated)
    {
        RuntimeContext::create()->activeDuring([&](){
            CHECK_EQUAL(LargeInteger{0}, wrapValue(LargeInteger{0})->perform<LargeInteger> ("negated"));
            CHECK_EQUAL(LargeInteger{-1}, wrapValue(LargeInteger{1})->perform<LargeInteger> ("negated"));
            CHECK_EQUAL(LargeInteger{1}, wrapValue(LargeInteger{-1})->perform<LargeInteger> ("negated"));

            CHECK_EQUAL(0, wrapValue(0)->perform<int> ("negated"));
            CHECK_EQUAL(-1, wrapValue(1)->perform<int> ("negated"));
            CHECK_EQUAL(1, wrapValue(-1)->perform<int> ("negated"));

            CHECK_EQUAL(-1.0f, wrapValue(1.0f)->perform<float> ("negated"));
            CHECK_EQUAL(-42.5f, wrapValue(42.5f)->perform<float> ("negated"));
            CHECK_EQUAL(1.0f, wrapValue(-1.0f)->perform<float> ("negated"));
            CHECK_EQUAL(42.5f, wrapValue(-42.5f)->perform<float> ("negated"));

            CHECK_EQUAL(-1.0, wrapValue(1.0)->perform<float> ("negated"));
            CHECK_EQUAL(-42.5, wrapValue(42.5)->perform<float> ("negated"));
            CHECK_EQUAL(1.0, wrapValue(-1.0)->perform<float> ("negated"));
            CHECK_EQUAL(42.5, wrapValue(-42.5)->perform<float> ("negated"));
        });
    }

    TEST(Addition)
    {
        RuntimeContext::create()->activeDuring([&](){
            // Integer - Integer
            CHECK_EQUAL(0, wrapValue(0)->perform<int> ("+", 0));
            CHECK_EQUAL(1, wrapValue(1)->perform<int> ("+", 0));
            CHECK_EQUAL(-1, wrapValue(-1)->perform<int> ("+", 0));

            CHECK_EQUAL(2, wrapValue(1)->perform<int> ("+", 1));
            CHECK_EQUAL(0, wrapValue(1)->perform<int> ("+", -1));

            // Integer - Fraction
            CHECK_EQUAL(Fraction(3, 2), wrapValue(Fraction(1, 2))->perform<Fraction> ("+", 1));

            // Fraction - Fraction
            CHECK_EQUAL(Fraction(3, 5), wrapValue(Fraction(1, 5))->perform<Fraction> ("+", Fraction(2, 5)));
            CHECK_EQUAL(1, wrapValue(Fraction(1, 2))->perform<int> ("+", Fraction(1, 2)));

            // Integer - Float
            CHECK_EQUAL(0.0, wrapValue(0)->perform<double> ("+", 0.0));
            CHECK_EQUAL(1.0, wrapValue(1)->perform<double> ("+", 0.0));
            CHECK_EQUAL(-1.0, wrapValue(-1)->perform<double> ("+", 0.0));

            CHECK_EQUAL(2.0, wrapValue(1)->perform<double> ("+", 1.0));
            CHECK_EQUAL(0.0, wrapValue(1)->perform<double> ("+", -1.0));

            // Float - Integer
            CHECK_EQUAL(0.0, wrapValue(0.0)->perform<double> ("+", 0));
            CHECK_EQUAL(1.0, wrapValue(1.0)->perform<double> ("+", 0));
            CHECK_EQUAL(-1.0, wrapValue(-1.0)->perform<double> ("+", 0));

            CHECK_EQUAL(2.0, wrapValue(1.0)->perform<double> ("+", 1));
            CHECK_EQUAL(0.0, wrapValue(1.0)->perform<double> ("+", -1));

            // Float - Float
            CHECK_EQUAL(0.0, wrapValue(0.0)->perform<double> ("+", 0.0));
            CHECK_EQUAL(1.0, wrapValue(1.0)->perform<double> ("+", 0.0));
            CHECK_EQUAL(-1.0, wrapValue(-1.0)->perform<double> ("+", 0.0));

            CHECK_EQUAL(2.0, wrapValue(1.0)->perform<double> ("+", 1.0));
            CHECK_EQUAL(0.0, wrapValue(1.0)->perform<double> ("+", -1.0));

        });
    }

    TEST(Subtraction)
    {
        RuntimeContext::create()->activeDuring([&](){
            // Integer - Integer
            CHECK_EQUAL(0, wrapValue(0)->perform<int> ("-", 0));
            CHECK_EQUAL(1, wrapValue(1)->perform<int> ("-", 0));
            CHECK_EQUAL(-1, wrapValue(-1)->perform<int> ("-", 0));

            CHECK_EQUAL(0, wrapValue(1)->perform<int> ("-", 1));
            CHECK_EQUAL(2, wrapValue(1)->perform<int> ("-", -1));

            // Integer - Float
            CHECK_EQUAL(0.0, wrapValue(0)->perform<double> ("-", 0.0));
            CHECK_EQUAL(1.0, wrapValue(1)->perform<double> ("-", 0.0));
            CHECK_EQUAL(-1.0, wrapValue(-1)->perform<double> ("-", 0.0));

            CHECK_EQUAL(0.0, wrapValue(1)->perform<double> ("-", 1.0));
            CHECK_EQUAL(2.0, wrapValue(1)->perform<double> ("-", -1.0));

            // Float - Integer
            CHECK_EQUAL(0.0, wrapValue(0.0)->perform<double> ("-", 0));
            CHECK_EQUAL(1.0, wrapValue(1.0)->perform<double> ("-", 0));
            CHECK_EQUAL(-1.0, wrapValue(-1.0)->perform<double> ("-", 0));

            CHECK_EQUAL(0.0, wrapValue(1.0)->perform<double> ("-", 1));
            CHECK_EQUAL(2.0, wrapValue(1.0)->perform<double> ("-", -1));

            // Float - Float
            CHECK_EQUAL(0.0, wrapValue(0.0)->perform<double> ("-", 0.0));
            CHECK_EQUAL(1.0, wrapValue(1.0)->perform<double> ("-", 0.0));
            CHECK_EQUAL(-1.0, wrapValue(-1.0)->perform<double> ("-", 0.0));

            CHECK_EQUAL(0.0, wrapValue(1.0)->perform<double> ("-", 1.0));
            CHECK_EQUAL(2.0, wrapValue(1.0)->perform<double> ("-", -1.0));
        });
    }

    TEST(Multiplication)
    {
        RuntimeContext::create()->activeDuring([&](){
            // Integer - Integer
            CHECK_EQUAL(0, wrapValue(0)->perform<int> ("*", 0));
            CHECK_EQUAL(0, wrapValue(1)->perform<int> ("*", 0));
            CHECK_EQUAL(0, wrapValue(-1)->perform<int> ("*", 0));

            CHECK_EQUAL(0, wrapValue(0)->perform<int> ("*", 1));
            CHECK_EQUAL(1, wrapValue(1)->perform<int> ("*", 1));
            CHECK_EQUAL(-1, wrapValue(-1)->perform<int> ("*", 1));

            CHECK_EQUAL(0, wrapValue(0)->perform<int> ("*", -1));
            CHECK_EQUAL(-1, wrapValue(1)->perform<int> ("*", -1));
            CHECK_EQUAL(1, wrapValue(-1)->perform<int> ("*", -1));

            CHECK_EQUAL(0, wrapValue(0)->perform<int> ("*", 2));
            CHECK_EQUAL(2, wrapValue(1)->perform<int> ("*", 2));
            CHECK_EQUAL(-2, wrapValue(-1)->perform<int> ("*", 2));

            // Integer - Float
            CHECK_EQUAL(0.0, wrapValue(0)->perform<double> ("*", 0.0));
            CHECK_EQUAL(0.0, wrapValue(1)->perform<double> ("*", 0.0));
            CHECK_EQUAL(0.0, wrapValue(-1)->perform<double> ("*", 0.0));

            CHECK_EQUAL(0.0, wrapValue(0)->perform<double> ("*", 1.0));
            CHECK_EQUAL(1.0, wrapValue(1)->perform<double> ("*", 1.0));
            CHECK_EQUAL(-1.0, wrapValue(-1)->perform<double> ("*", 1.0));

            CHECK_EQUAL(0.0, wrapValue(0)->perform<double> ("*", -1.0));
            CHECK_EQUAL(-1.0, wrapValue(1)->perform<double> ("*", -1.0));
            CHECK_EQUAL(1.0, wrapValue(-1)->perform<double> ("*", -1.0));

            CHECK_EQUAL(0.0, wrapValue(0)->perform<double> ("*", 2.0));
            CHECK_EQUAL(2.0, wrapValue(1)->perform<double> ("*", 2.0));
            CHECK_EQUAL(-2.0, wrapValue(-1)->perform<double> ("*", 2.0));

            // Float - Integer
            CHECK_EQUAL(0.0, wrapValue(0.0)->perform<double> ("*", 0));
            CHECK_EQUAL(0.0, wrapValue(1.0)->perform<double> ("*", 0));
            CHECK_EQUAL(0.0, wrapValue(-1.0)->perform<double> ("*", 0));

            CHECK_EQUAL(0.0, wrapValue(0.0)->perform<double> ("*", 1));
            CHECK_EQUAL(1.0, wrapValue(1.0)->perform<double> ("*", 1));
            CHECK_EQUAL(-1.0, wrapValue(-1.0)->perform<double> ("*", 1));

            CHECK_EQUAL(0.0, wrapValue(0.0)->perform<double> ("*", -1));
            CHECK_EQUAL(-1.0, wrapValue(1.0)->perform<double> ("*", -1));
            CHECK_EQUAL(1.0, wrapValue(-1.0)->perform<double> ("*", -1));

            CHECK_EQUAL(0.0, wrapValue(0.0)->perform<double> ("*", 2));
            CHECK_EQUAL(2.0, wrapValue(1.0)->perform<double> ("*", 2));
            CHECK_EQUAL(-2.0, wrapValue(-1.0)->perform<double> ("*", 2));

            // Float - Float
            CHECK_EQUAL(0.0, wrapValue(0.0)->perform<double> ("*", 0.0));
            CHECK_EQUAL(0.0, wrapValue(1.0)->perform<double> ("*", 0.0));
            CHECK_EQUAL(0.0, wrapValue(-1.0)->perform<double> ("*", 0.0));

            CHECK_EQUAL(0.0, wrapValue(0.0)->perform<double> ("*", 1.0));
            CHECK_EQUAL(1.0, wrapValue(1.0)->perform<double> ("*", 1.0));
            CHECK_EQUAL(-1.0, wrapValue(-1.0)->perform<double> ("*", 1.0));

            CHECK_EQUAL(0.0, wrapValue(0.0)->perform<double> ("*", -1.0));
            CHECK_EQUAL(-1.0, wrapValue(1.0)->perform<double> ("*", -1.0));
            CHECK_EQUAL(1.0, wrapValue(-1.0)->perform<double> ("*", -1.0));

            CHECK_EQUAL(0.0, wrapValue(0.0)->perform<double> ("*", 2.0));
            CHECK_EQUAL(2.0, wrapValue(1.0)->perform<double> ("*", 2.0));
            CHECK_EQUAL(-2.0, wrapValue(-1.0)->perform<double> ("*", 2.0));
        });
    }

    TEST(Division)
    {
        RuntimeContext::create()->activeDuring([&](){
            // Integer - Integer => Integer
            CHECK_EQUAL(0, wrapValue(0)->perform<int> ("/", 1));
            CHECK_EQUAL(0, wrapValue(0)->perform<int> ("/", -1));

            CHECK_EQUAL(2, wrapValue(6)->perform<int> ("/", 3));
            CHECK_EQUAL(-3, wrapValue(6)->perform<int> ("/", -2));
            CHECK_EQUAL(-3, wrapValue(-6)->perform<int> ("/", 2));

            // Integer - Float => Float
            CHECK_EQUAL(0.0, wrapValue(0)->perform<double> ("/", 1.0));
            CHECK_EQUAL(0.0, wrapValue(0)->perform<double> ("/", -1.0));

            CHECK_EQUAL(2.0, wrapValue(6)->perform<double> ("/", 3.0));
            CHECK_EQUAL(-3.0, wrapValue(6)->perform<double> ("/", -2.0));
            CHECK_EQUAL(-3.0, wrapValue(-6)->perform<double> ("/", 2.0));

            // Float - Integer => Float
            CHECK_EQUAL(0.0, wrapValue(0.0)->perform<double> ("/", 1));
            CHECK_EQUAL(0.0, wrapValue(0.0)->perform<double> ("/", -1));

            CHECK_EQUAL(2.0, wrapValue(6.0)->perform<double> ("/", 3));
            CHECK_EQUAL(-3.0, wrapValue(6.0)->perform<double> ("/", -2));
            CHECK_EQUAL(-3.0, wrapValue(-6.0)->perform<double> ("/", 2));

            // Float - Float => Float
            CHECK_EQUAL(0.0, wrapValue(0.0)->perform<double> ("/", 1.0));
            CHECK_EQUAL(0.0, wrapValue(0.0)->perform<double> ("/", -1.0));

            CHECK_EQUAL(2.0, wrapValue(6.0)->perform<double> ("/", 3.0));
            CHECK_EQUAL(-3.0, wrapValue(6.0)->perform<double> ("/", -2.0));
            CHECK_EQUAL(-3.0, wrapValue(-6.0)->perform<double> ("/", 2.0));

        });
    }

    TEST(IntegerDivision)
    {
        RuntimeContext::create()->activeDuring([&](){
            CHECK_EQUAL(0, wrapValue(0)->perform<int> ("//", 1));
            CHECK_EQUAL(0, wrapValue(0)->perform<int> ("//", -1));

            CHECK_EQUAL(2, wrapValue(6)->perform<int> ("//", 3));
            CHECK_EQUAL(-3, wrapValue(6)->perform<int> ("//", -2));
            CHECK_EQUAL(-3, wrapValue(-6)->perform<int> ("//", 2));
        });
    }

    TEST(IntegerRemainder)
    {
        RuntimeContext::create()->activeDuring([&](){
            CHECK_EQUAL(0, wrapValue(0)->perform<int> ("%", 1));
            CHECK_EQUAL(0, wrapValue(0)->perform<int> ("%", -1));

            CHECK_EQUAL(0, wrapValue(6)->perform<int> ("%", 3));
            CHECK_EQUAL(0, wrapValue(6)->perform<int> ("%", -2));
            CHECK_EQUAL(0, wrapValue(-6)->perform<int> ("%", 2));

            CHECK_EQUAL(0, wrapValue(0)->perform<int> ("%", 3));
            CHECK_EQUAL(1, wrapValue(1)->perform<int> ("%", 3));
            CHECK_EQUAL(2, wrapValue(2)->perform<int> ("%", 3));
            CHECK_EQUAL(0, wrapValue(3)->perform<int> ("%", 3));
            CHECK_EQUAL(1, wrapValue(4)->perform<int> ("%", 3));
            CHECK_EQUAL(2, wrapValue(5)->perform<int> ("%", 3));

            CHECK_EQUAL(0, wrapValue(0)->perform<int> ("%", -3));
            CHECK_EQUAL(1, wrapValue(1)->perform<int> ("%", -3));
            CHECK_EQUAL(2, wrapValue(2)->perform<int> ("%", -3));
            CHECK_EQUAL(0, wrapValue(3)->perform<int> ("%", -3));
            CHECK_EQUAL(1, wrapValue(4)->perform<int> ("%", -3));
            CHECK_EQUAL(2, wrapValue(5)->perform<int> ("%", -3));

            CHECK_EQUAL(-1, wrapValue(-1)->perform<int> ("%", 3));
            CHECK_EQUAL(-2, wrapValue(-2)->perform<int> ("%", 3));
            CHECK_EQUAL(-0, wrapValue(-3)->perform<int> ("%", 3));
            CHECK_EQUAL(-1, wrapValue(-4)->perform<int> ("%", 3));
            CHECK_EQUAL(-2, wrapValue(-5)->perform<int> ("%", 3));

            CHECK_EQUAL(-1, wrapValue(-1)->perform<int> ("%", -3));
            CHECK_EQUAL(-2, wrapValue(-2)->perform<int> ("%", -3));
            CHECK_EQUAL(-0, wrapValue(-3)->perform<int> ("%", -3));
            CHECK_EQUAL(-1, wrapValue(-4)->perform<int> ("%", -3));
            CHECK_EQUAL(-2, wrapValue(-5)->perform<int> ("%", -3));
        });
    }

    TEST(IntegerRemainder2)
    {
        RuntimeContext::create()->activeDuring([&](){
            CHECK_EQUAL(0, wrapValue(0)->perform<int> ("\\\\", 1));
            CHECK_EQUAL(0, wrapValue(0)->perform<int> ("\\\\", -1));

            CHECK_EQUAL(0, wrapValue(6)->perform<int> ("\\\\", 3));
            CHECK_EQUAL(0, wrapValue(6)->perform<int> ("\\\\", -2));
            CHECK_EQUAL(0, wrapValue(-6)->perform<int> ("\\\\", 2));

            CHECK_EQUAL(0, wrapValue(0)->perform<int> ("\\\\", 3));
            CHECK_EQUAL(1, wrapValue(1)->perform<int> ("\\\\", 3));
            CHECK_EQUAL(2, wrapValue(2)->perform<int> ("\\\\", 3));
            CHECK_EQUAL(0, wrapValue(3)->perform<int> ("\\\\", 3));
            CHECK_EQUAL(1, wrapValue(4)->perform<int> ("\\\\", 3));
            CHECK_EQUAL(2, wrapValue(5)->perform<int> ("\\\\", 3));

            CHECK_EQUAL(0, wrapValue(0)->perform<int> ("\\\\", -3));
            CHECK_EQUAL(1, wrapValue(1)->perform<int> ("\\\\", -3));
            CHECK_EQUAL(2, wrapValue(2)->perform<int> ("\\\\", -3));
            CHECK_EQUAL(0, wrapValue(3)->perform<int> ("\\\\", -3));
            CHECK_EQUAL(1, wrapValue(4)->perform<int> ("\\\\", -3));
            CHECK_EQUAL(2, wrapValue(5)->perform<int> ("\\\\", -3));

            CHECK_EQUAL(-1, wrapValue(-1)->perform<int> ("\\\\", 3));
            CHECK_EQUAL(-2, wrapValue(-2)->perform<int> ("\\\\", 3));
            CHECK_EQUAL(-0, wrapValue(-3)->perform<int> ("\\\\", 3));
            CHECK_EQUAL(-1, wrapValue(-4)->perform<int> ("\\\\", 3));
            CHECK_EQUAL(-2, wrapValue(-5)->perform<int> ("\\\\", 3));

            CHECK_EQUAL(-1, wrapValue(-1)->perform<int> ("\\\\", -3));
            CHECK_EQUAL(-2, wrapValue(-2)->perform<int> ("\\\\", -3));
            CHECK_EQUAL(-0, wrapValue(-3)->perform<int> ("\\\\", -3));
            CHECK_EQUAL(-1, wrapValue(-4)->perform<int> ("\\\\", -3));
            CHECK_EQUAL(-2, wrapValue(-5)->perform<int> ("\\\\", -3));
        });
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
