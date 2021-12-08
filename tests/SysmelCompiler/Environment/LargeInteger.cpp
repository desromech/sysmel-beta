#include "Environment/LargeInteger.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/Exception.hpp"
#include "UnitTest++/UnitTest++.h"
#include <limits>

using namespace Sysmel::Environment;

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

    TEST(Subtraction)
    {
        // Neutral element
        CHECK_EQUAL(LargeInteger{0}, LargeInteger{0} - LargeInteger{0});
        CHECK_EQUAL(LargeInteger{1}, LargeInteger{1} - LargeInteger{0});

        CHECK_EQUAL(LargeInteger{-1}, LargeInteger{0} - LargeInteger{1});

        // Simple subtractions.
        CHECK_EQUAL(LargeInteger{0}, LargeInteger{1} - LargeInteger{1});
        CHECK_EQUAL(LargeInteger{-35}, LargeInteger{7} - LargeInteger{42});
 
        CHECK_EQUAL(LargeInteger{5}, LargeInteger{2} - LargeInteger{-3});
        CHECK_EQUAL(LargeInteger{5}, LargeInteger{3} - LargeInteger{-2});
        CHECK_EQUAL(LargeInteger{-2}, LargeInteger{-1} - LargeInteger{1});
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

    TEST(Division)
    {
        // Neutral element
        CHECK_EQUAL(LargeInteger{2}, LargeInteger{2} / LargeInteger{1});
        CHECK_EQUAL(LargeInteger{3}, LargeInteger{3} / LargeInteger{1});
        CHECK_EQUAL(LargeInteger{-2}, LargeInteger{-2} / LargeInteger{1});
        CHECK_EQUAL(LargeInteger{-3}, LargeInteger{-3} / LargeInteger{1});
        
        // Division from zero
        CHECK_EQUAL(LargeInteger{0}, LargeInteger{0} / LargeInteger{1});
        CHECK_EQUAL(LargeInteger{0}, LargeInteger{0} / LargeInteger{-1});
        CHECK_EQUAL(LargeInteger{0}, LargeInteger{0} / LargeInteger{2});
        CHECK_EQUAL(LargeInteger{0}, LargeInteger{0} / LargeInteger{-2});

        // Negation
        CHECK_EQUAL(LargeInteger{1}, LargeInteger{-1} / LargeInteger{-1});
        CHECK_EQUAL(LargeInteger{-1}, LargeInteger{-1} / LargeInteger{1});
        CHECK_EQUAL(LargeInteger{-1}, LargeInteger{1} / LargeInteger{-1});
        CHECK_EQUAL(LargeInteger{-2}, LargeInteger{2} / LargeInteger{-1});

        // Simple divisions
        CHECK_EQUAL(LargeInteger{3}, LargeInteger{6} / LargeInteger{2});
        CHECK_EQUAL(LargeInteger{2}, LargeInteger{6} / LargeInteger{3});
        CHECK_EQUAL(LargeInteger{7}, LargeInteger{49} / LargeInteger{7});
    }

    TEST(Remainder)
    {
        // Positive - Positive
        CHECK_EQUAL(0, 0 % 5); CHECK_EQUAL(LargeInteger{0}, LargeInteger{0} % LargeInteger{5});
        CHECK_EQUAL(1, 1 % 5); CHECK_EQUAL(LargeInteger{1}, LargeInteger{1} % LargeInteger{5});
        CHECK_EQUAL(2, 2 % 5); CHECK_EQUAL(LargeInteger{2}, LargeInteger{2} % LargeInteger{5});
        CHECK_EQUAL(3, 3 % 5); CHECK_EQUAL(LargeInteger{3}, LargeInteger{3} % LargeInteger{5});
        CHECK_EQUAL(4, 4 % 5); CHECK_EQUAL(LargeInteger{4}, LargeInteger{4} % LargeInteger{5});
        CHECK_EQUAL(0, 5 % 5); CHECK_EQUAL(LargeInteger{0}, LargeInteger{5} % LargeInteger{5});
        CHECK_EQUAL(1, 6 % 5); CHECK_EQUAL(LargeInteger{1}, LargeInteger{6} % LargeInteger{5});
        CHECK_EQUAL(2, 7 % 5); CHECK_EQUAL(LargeInteger{2}, LargeInteger{7} % LargeInteger{5});
        CHECK_EQUAL(3, 8 % 5); CHECK_EQUAL(LargeInteger{3}, LargeInteger{8} % LargeInteger{5});
        CHECK_EQUAL(4, 9 % 5); CHECK_EQUAL(LargeInteger{4}, LargeInteger{9} % LargeInteger{5});

        // Negative - Negative
        CHECK_EQUAL(-1, -1 % -5); CHECK_EQUAL(LargeInteger{-1}, LargeInteger{-1} % LargeInteger{-5});
        CHECK_EQUAL(-2, -2 % -5); CHECK_EQUAL(LargeInteger{-2}, LargeInteger{-2} % LargeInteger{-5});
        CHECK_EQUAL(-3, -3 % -5); CHECK_EQUAL(LargeInteger{-3}, LargeInteger{-3} % LargeInteger{-5});
        CHECK_EQUAL(-4, -4 % -5); CHECK_EQUAL(LargeInteger{-4}, LargeInteger{-4} % LargeInteger{-5});
        CHECK_EQUAL(0, -5 % -5); CHECK_EQUAL(LargeInteger{0}, LargeInteger{-5} % LargeInteger{-5});
        CHECK_EQUAL(-1, -6 % -5); CHECK_EQUAL(LargeInteger{-1}, LargeInteger{-6} % LargeInteger{-5});
        CHECK_EQUAL(-2, -7 % -5); CHECK_EQUAL(LargeInteger{-2}, LargeInteger{-7} % LargeInteger{-5});
        CHECK_EQUAL(-3, -8 % -5); CHECK_EQUAL(LargeInteger{-3}, LargeInteger{-8} % LargeInteger{-5});
        CHECK_EQUAL(-4, -9 % -5); CHECK_EQUAL(LargeInteger{-4}, LargeInteger{-9} % LargeInteger{-5});

        // Positive - Negative
        CHECK_EQUAL(0, 0 % -5); CHECK_EQUAL(LargeInteger{0}, LargeInteger{0} % LargeInteger{-5});
        CHECK_EQUAL(1, 1 % -5); CHECK_EQUAL(LargeInteger{1}, LargeInteger{1} % LargeInteger{-5});
        CHECK_EQUAL(2, 2 % -5); CHECK_EQUAL(LargeInteger{2}, LargeInteger{2} % LargeInteger{-5});
        CHECK_EQUAL(3, 3 % -5); CHECK_EQUAL(LargeInteger{3}, LargeInteger{3} % LargeInteger{-5});
        CHECK_EQUAL(4, 4 % -5); CHECK_EQUAL(LargeInteger{4}, LargeInteger{4} % LargeInteger{-5});
        CHECK_EQUAL(0, 5 % -5); CHECK_EQUAL(LargeInteger{0}, LargeInteger{5} % LargeInteger{-5});
        CHECK_EQUAL(1, 6 % -5); CHECK_EQUAL(LargeInteger{1}, LargeInteger{6} % LargeInteger{-5});
        CHECK_EQUAL(2, 7 % -5); CHECK_EQUAL(LargeInteger{2}, LargeInteger{7} % LargeInteger{-5});
        CHECK_EQUAL(3, 8 % -5); CHECK_EQUAL(LargeInteger{3}, LargeInteger{8} % LargeInteger{-5});
        CHECK_EQUAL(4, 9 % -5); CHECK_EQUAL(LargeInteger{4}, LargeInteger{9} % LargeInteger{-5});

        // Negative - Positive
        CHECK_EQUAL(-1, -1 % 5); CHECK_EQUAL(LargeInteger{-1}, LargeInteger{-1} % LargeInteger{5});
        CHECK_EQUAL(-2, -2 % 5); CHECK_EQUAL(LargeInteger{-2}, LargeInteger{-2} % LargeInteger{5});
        CHECK_EQUAL(-3, -3 % 5); CHECK_EQUAL(LargeInteger{-3}, LargeInteger{-3} % LargeInteger{5});
        CHECK_EQUAL(-4, -4 % 5); CHECK_EQUAL(LargeInteger{-4}, LargeInteger{-4} % LargeInteger{5});
        CHECK_EQUAL(0, -5 % 5); CHECK_EQUAL(LargeInteger{0}, LargeInteger{-5} % LargeInteger{5});
        CHECK_EQUAL(-1, -6 % 5); CHECK_EQUAL(LargeInteger{-1}, LargeInteger{-6} % LargeInteger{5});
        CHECK_EQUAL(-2, -7 % 5); CHECK_EQUAL(LargeInteger{-2}, LargeInteger{-7} % LargeInteger{5});
        CHECK_EQUAL(-3, -8 % 5); CHECK_EQUAL(LargeInteger{-3}, LargeInteger{-8} % LargeInteger{5});
        CHECK_EQUAL(-4, -9 % 5); CHECK_EQUAL(LargeInteger{-4}, LargeInteger{-9} % LargeInteger{5});
    }

    TEST(DivisionByZeroError)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            CHECK_THROW(LargeInteger{1}/LargeInteger{0}, ExceptionWrapper);
            CHECK_THROW(LargeInteger{0}/LargeInteger{0}, ExceptionWrapper);
        });
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

        CHECK_EQUAL(LargeInteger{uint64_t(2432902008176640000)}, LargeInteger{20}.factorial());
        CHECK_EQUAL(LargeInteger{"265252859812191058636308480000000"}, LargeInteger{30}.factorial());
        CHECK_EQUAL(LargeInteger{"815915283247897734345611269596115894272000000000"}, LargeInteger{40}.factorial());
        CHECK_EQUAL(LargeInteger{"30414093201713378043612608166064768844377641568960512000000000000"}, LargeInteger{50}.factorial());
        CHECK_EQUAL(LargeInteger{"93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000"}, LargeInteger{100}.factorial());
    }

    TEST(BinomialCoefficient)
    {
        CHECK_EQUAL(LargeInteger{1}, LargeInteger::binomialCoefficient(LargeInteger{0}, LargeInteger{0}));

        CHECK_EQUAL(LargeInteger{1}, LargeInteger::binomialCoefficient(LargeInteger{1}, LargeInteger{0}));
        CHECK_EQUAL(LargeInteger{1}, LargeInteger::binomialCoefficient(LargeInteger{1}, LargeInteger{1}));

        CHECK_EQUAL(LargeInteger{1}, LargeInteger::binomialCoefficient(LargeInteger{2}, LargeInteger{0}));
        CHECK_EQUAL(LargeInteger{2}, LargeInteger::binomialCoefficient(LargeInteger{2}, LargeInteger{1}));
        CHECK_EQUAL(LargeInteger{1}, LargeInteger::binomialCoefficient(LargeInteger{2}, LargeInteger{2}));

        CHECK_EQUAL(LargeInteger{1}, LargeInteger::binomialCoefficient(LargeInteger{3}, LargeInteger{0}));
        CHECK_EQUAL(LargeInteger{3}, LargeInteger::binomialCoefficient(LargeInteger{3}, LargeInteger{1}));
        CHECK_EQUAL(LargeInteger{3}, LargeInteger::binomialCoefficient(LargeInteger{3}, LargeInteger{2}));
        CHECK_EQUAL(LargeInteger{1}, LargeInteger::binomialCoefficient(LargeInteger{3}, LargeInteger{3}));

        CHECK_EQUAL(LargeInteger{1}, LargeInteger::binomialCoefficient(LargeInteger{4}, LargeInteger{0}));
        CHECK_EQUAL(LargeInteger{4}, LargeInteger::binomialCoefficient(LargeInteger{4}, LargeInteger{1}));
        CHECK_EQUAL(LargeInteger{6}, LargeInteger::binomialCoefficient(LargeInteger{4}, LargeInteger{2}));
        CHECK_EQUAL(LargeInteger{4}, LargeInteger::binomialCoefficient(LargeInteger{4}, LargeInteger{3}));
        CHECK_EQUAL(LargeInteger{1}, LargeInteger::binomialCoefficient(LargeInteger{4}, LargeInteger{4}));
    }

    TEST(AsHexString)
    {
        CHECK_EQUAL("0", LargeInteger{}.asHexString());
        CHECK_EQUAL("1", LargeInteger{1}.asHexString());
        CHECK_EQUAL("-1", LargeInteger{-1}.asHexString());
        CHECK_EQUAL("A", LargeInteger{10}.asHexString());
        CHECK_EQUAL("2A", LargeInteger{42}.asHexString());
        CHECK_EQUAL("1E240", LargeInteger{123456}.asHexString());

        CHECK_EQUAL("21C3677C82B40000", LargeInteger{20}.factorial().asHexString());
        CHECK_EQUAL("D13F6370F96865DF5DD54000000", LargeInteger{30}.factorial().asHexString());
        CHECK_EQUAL("8EEAE81B84C7F27E080FDE64FF05254000000000", LargeInteger{40}.factorial().asHexString());
        CHECK_EQUAL("49EEBC961ED279B02B1EF4F28D19A84F5973A1D2C7800000000000", LargeInteger{50}.factorial().asHexString());
        CHECK_EQUAL("1B30964EC395DC24069528D54BBDA40D16E966EF9A70EB21B5B2943A321CDF10391745570CCA9420C6ECB3B72ED2EE8B02EA2735C61A000000000000000000000000", LargeInteger{100}.factorial().asHexString());
        CHECK_EQUAL("29A857090114875BA4DB6CD48EB197FFCAF0730BC388499A8C0E87E9854C253EEB306EF59E33ECC0F352847EAAE8691E58A9BEB434E84A9D68FBAB85E3270936F66CD32B2009B4948A51C096C970FD7F3E208773A39A9BD13A95CA25CE6EDF52F45A0A019B4431524593E57012C03AD01E8372C59E8A8ADF4C076D40B09CB04C48AEC2A0000000000000000000000000000000000000000000000000", LargeInteger{200}.factorial().asHexString());
    }

    TEST(AsString)
    {
        CHECK_EQUAL("0", LargeInteger{}.asString());
        CHECK_EQUAL("1", LargeInteger{1}.asString());
        CHECK_EQUAL("-1", LargeInteger{-1}.asString());
        CHECK_EQUAL("10", LargeInteger{10}.asString());
        CHECK_EQUAL("42", LargeInteger{42}.asString());
        CHECK_EQUAL("123456", LargeInteger{123456}.asString());

        CHECK_EQUAL("2432902008176640000", LargeInteger{20}.factorial().asString());
        CHECK_EQUAL("265252859812191058636308480000000", LargeInteger{30}.factorial().asString());
        CHECK_EQUAL("815915283247897734345611269596115894272000000000", LargeInteger{40}.factorial().asString());
        CHECK_EQUAL("30414093201713378043612608166064768844377641568960512000000000000", LargeInteger{50}.factorial().asString());
        CHECK_EQUAL("93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000", LargeInteger{100}.factorial().asString());
        CHECK_EQUAL("788657867364790503552363213932185062295135977687173263294742533244359449963403342920304284011984623904177212138919638830257642790242637105061926624952829931113462857270763317237396988943922445621451664240254033291864131227428294853277524242407573903240321257405579568660226031904170324062351700858796178922222789623703897374720000000000000000000000000000000000000000000000000", LargeInteger{200}.factorial().asString());
    }

    TEST(AsDouble)
    {
        CHECK_EQUAL(0.0, LargeInteger{}.asDouble());
        CHECK_EQUAL(1.0, LargeInteger{1}.asDouble());
        CHECK_EQUAL(-1.0, LargeInteger{-1}.asDouble());
        CHECK_EQUAL(10.0, LargeInteger{10}.asDouble());
        CHECK_EQUAL(42.0, LargeInteger{42}.asDouble());
        CHECK_EQUAL(123456.0, LargeInteger{123456}.asDouble());

        CHECK_EQUAL(2.43290200817664e18, LargeInteger{20}.factorial().asDouble());
        CHECK_EQUAL(2.6525285981219107e32, LargeInteger{30}.factorial().asDouble());
        CHECK_EQUAL(8.159152832478977e47, LargeInteger{40}.factorial().asDouble());
        CHECK_EQUAL(3.0414093201713376e64, LargeInteger{50}.factorial().asDouble());
        CHECK_EQUAL(9.332621544394415e157, LargeInteger{100}.factorial().asDouble());
        CHECK_EQUAL(std::numeric_limits<double>::infinity(), LargeInteger{200}.factorial().asDouble());
    }

}
