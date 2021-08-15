#include "sysmel/ObjectModel/LargeInteger.hpp"
#include "UnitTest++/UnitTest++.h"
#include <iostream>

using namespace SysmelMoebius::ObjectModel;

SUITE(LargeInteger)
{
    TEST(Default)
    {
        CHECK_EQUAL(LargeInteger{0}, LargeInteger{});
        CHECK_EQUAL(0u, LargeInteger{}.words.size());
        CHECK_EQUAL(0u, LargeInteger{0}.words.size());
        CHECK_EQUAL(0u, LargeInteger{0}.wordAt(0));
        CHECK_EQUAL(0, LargeInteger{0}.sign());
    }

    TEST(Int32Constructor)
    {
        CHECK_EQUAL(LargeInteger::Zero, LargeInteger{0});
        CHECK_EQUAL(LargeInteger::Zero, LargeInteger{"0"});
        CHECK_EQUAL(LargeInteger::MinusOne, LargeInteger{"-1"});

        CHECK_EQUAL(LargeInteger::One, LargeInteger{1});
        CHECK_EQUAL(LargeInteger::One, LargeInteger{"1"});
        CHECK_EQUAL(1u, LargeInteger{1}.words.size());
        CHECK_EQUAL(1u, LargeInteger{1}.words.front());
        CHECK_EQUAL(1u, LargeInteger{1}.wordAt(0));
        CHECK_EQUAL(1, LargeInteger{1}.sign());

        CHECK_EQUAL(LargeInteger::Ten, LargeInteger{10});
        CHECK_EQUAL(LargeInteger::Ten, LargeInteger{"10"});
        CHECK_EQUAL(LargeInteger::Ten, (LargeInteger{"1010", 2}));
        CHECK_EQUAL(LargeInteger::Ten, (LargeInteger{"a", 16}));
        CHECK_EQUAL(LargeInteger::Ten, (LargeInteger{"A", 16}));
        CHECK_EQUAL(10u, LargeInteger{10}.words.front());
        CHECK_EQUAL(10u, LargeInteger{10}.words.front());
        CHECK_EQUAL(10u, LargeInteger{10}.wordAt(0));
        CHECK_EQUAL(1, LargeInteger{10}.sign());
    }

    TEST(NamedConstants)
    {
        CHECK_EQUAL(LargeInteger{0}, LargeInteger{0});
        CHECK_EQUAL(LargeInteger::Zero, LargeInteger{0});
        CHECK(LargeInteger{}.isZero());
        CHECK(!LargeInteger{}.isOne());
        CHECK(!LargeInteger{}.isMinusOne());

        CHECK_EQUAL(LargeInteger{1}, LargeInteger{1});
        CHECK_EQUAL(LargeInteger::One, LargeInteger{1});
        CHECK(!LargeInteger{1}.isZero());
        CHECK(LargeInteger{1}.isOne());
        CHECK(!LargeInteger{1}.isMinusOne());
        
        CHECK_EQUAL(LargeInteger{-1}, LargeInteger{-1});
        CHECK_EQUAL(LargeInteger::MinusOne, LargeInteger{-1});
        CHECK(!LargeInteger{-1}.isZero());
        CHECK(!LargeInteger{-1}.isOne());
        CHECK(LargeInteger{-1}.isMinusOne());

        CHECK_EQUAL(LargeInteger{10}, LargeInteger{10});
        CHECK_EQUAL(LargeInteger::Ten, LargeInteger{10});
    }

    TEST(Negation)
    {
        CHECK_EQUAL(LargeInteger{0}, -LargeInteger{0});

        CHECK_EQUAL(LargeInteger{-1}, -LargeInteger{1});
        CHECK_EQUAL(LargeInteger{1}, -LargeInteger{-1});

        CHECK_EQUAL(LargeInteger{-2}, -LargeInteger{2});
        CHECK_EQUAL(LargeInteger{2}, -LargeInteger{-2});
    }

    TEST(Addition)
    {
        // Neutral element
        CHECK_EQUAL(LargeInteger{0}, LargeInteger{0} + LargeInteger{0});
        CHECK_EQUAL(LargeInteger{1}, LargeInteger{0} + LargeInteger{1});
        CHECK_EQUAL(LargeInteger{1}, LargeInteger{1} + LargeInteger{0});

        // Cancellation
        CHECK_EQUAL(LargeInteger{0}, LargeInteger{-1} + LargeInteger{1});

        // Simple sums.
        CHECK_EQUAL(LargeInteger{2}, LargeInteger{1} + LargeInteger{1});
        CHECK_EQUAL(LargeInteger{49}, LargeInteger{7} + LargeInteger{42});

        // Differing sign sums
        CHECK_EQUAL(LargeInteger{-1}, LargeInteger{2} + LargeInteger{-3});
        CHECK_EQUAL(LargeInteger{1}, LargeInteger{3} + LargeInteger{-2});
    }

    TEST(Multiplication)
    {
        // Neutral element
        CHECK_EQUAL(LargeInteger{1}, LargeInteger{1} * LargeInteger{1});
        CHECK_EQUAL(LargeInteger{2}, LargeInteger{2} * LargeInteger{1});
        CHECK_EQUAL(LargeInteger{2}, LargeInteger{1} * LargeInteger{2});
        CHECK_EQUAL(LargeInteger{3}, LargeInteger{3} * LargeInteger{1});
        CHECK_EQUAL(LargeInteger{3}, LargeInteger{1} * LargeInteger{3});
        CHECK_EQUAL(LargeInteger{-2}, LargeInteger{-2} * LargeInteger{1});
        CHECK_EQUAL(LargeInteger{-2}, LargeInteger{1} * LargeInteger{-2});
        CHECK_EQUAL(LargeInteger{-3}, LargeInteger{-3} * LargeInteger{1});
        CHECK_EQUAL(LargeInteger{-3}, LargeInteger{1} * LargeInteger{-3});
        
        // Absorbing element
        CHECK_EQUAL(LargeInteger{0}, LargeInteger{0} * LargeInteger{1});
        CHECK_EQUAL(LargeInteger{0}, LargeInteger{1} * LargeInteger{0});
        CHECK_EQUAL(LargeInteger{0}, LargeInteger{0} * LargeInteger{-1});
        CHECK_EQUAL(LargeInteger{0}, LargeInteger{1} * LargeInteger{0});

        // Negation
        CHECK_EQUAL(LargeInteger{1}, LargeInteger{-1} * LargeInteger{-1});
        CHECK_EQUAL(LargeInteger{-1}, LargeInteger{-1} * LargeInteger{1});
        CHECK_EQUAL(LargeInteger{-1}, LargeInteger{1} * LargeInteger{-1});
        CHECK_EQUAL(LargeInteger{-2}, LargeInteger{-1} * LargeInteger{2});
        CHECK_EQUAL(LargeInteger{-2}, LargeInteger{2} * LargeInteger{-1});

        // Simple multiplications.
        CHECK_EQUAL(LargeInteger{6}, LargeInteger{2} * LargeInteger{3});
        CHECK_EQUAL(LargeInteger{49}, LargeInteger{7} * LargeInteger{7});
    }

    TEST(ShiftLeft)
    {
        CHECK_EQUAL(LargeInteger{0}, LargeInteger{0} << 0);
        CHECK_EQUAL(LargeInteger{0}, LargeInteger{0} << 1);

        CHECK_EQUAL(LargeInteger{1}, LargeInteger{1} << 0);
        CHECK_EQUAL(LargeInteger{2}, LargeInteger{1} << 1);
        CHECK_EQUAL(LargeInteger{4}, LargeInteger{1} << 2);
        CHECK_EQUAL(LargeInteger{8}, LargeInteger{1} << 3);
    }

    TEST(ShiftRight)
    {
        CHECK_EQUAL(LargeInteger{0}, LargeInteger{0} >> 0);
        CHECK_EQUAL(LargeInteger{0}, LargeInteger{0} >> 1);

        CHECK_EQUAL(LargeInteger{0}, LargeInteger{1} >> 1);
        CHECK_EQUAL(LargeInteger{0}, LargeInteger{-1} >> 1);

        CHECK_EQUAL(LargeInteger{1}, LargeInteger{1} >> 0);
        CHECK_EQUAL(LargeInteger{1}, LargeInteger{2} >> 1);
        CHECK_EQUAL(LargeInteger{1}, LargeInteger{4} >> 2);
        CHECK_EQUAL(LargeInteger{1}, LargeInteger{8} >> 3);
    }

    TEST(Factorial)
    {
        CHECK_EQUAL(LargeInteger{1}, LargeInteger{0}.factorial());
        CHECK_EQUAL(LargeInteger{1}, LargeInteger{1}.factorial());
        CHECK_EQUAL(LargeInteger{2}, LargeInteger{2}.factorial());
        CHECK_EQUAL(LargeInteger{6}, LargeInteger{3}.factorial());
        CHECK_EQUAL(LargeInteger{24}, LargeInteger{4}.factorial());
        CHECK_EQUAL(LargeInteger{120}, LargeInteger{5}.factorial());
        CHECK_EQUAL(LargeInteger{720}, LargeInteger{6}.factorial());
        CHECK_EQUAL(LargeInteger{5040}, LargeInteger{7}.factorial());
        CHECK_EQUAL(LargeInteger{40320}, LargeInteger{8}.factorial());
        CHECK_EQUAL(LargeInteger{362880}, LargeInteger{9}.factorial());
        CHECK_EQUAL(LargeInteger{3628800}, LargeInteger{10}.factorial());

        CHECK_EQUAL(LargeInteger{2432902008176640000}, LargeInteger{20}.factorial());
        CHECK_EQUAL(LargeInteger{"265252859812191058636308480000000"}, LargeInteger{30}.factorial());
        CHECK_EQUAL(LargeInteger{"815915283247897734345611269596115894272000000000"}, LargeInteger{40}.factorial());
        CHECK_EQUAL(LargeInteger{"30414093201713378043612608166064768844377641568960512000000000000"}, LargeInteger{50}.factorial());
        CHECK_EQUAL(LargeInteger{"93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000"}, LargeInteger{100}.factorial());
    }

    TEST(AsString)
    {
        CHECK_EQUAL("0", LargeInteger{}.asString());
        CHECK_EQUAL("1", LargeInteger{1}.asString());
        CHECK_EQUAL("-1", LargeInteger{-1}.asString());
        CHECK_EQUAL("10", LargeInteger{10}.asString());
        CHECK_EQUAL("42", LargeInteger{42}.asString());
    }

}
