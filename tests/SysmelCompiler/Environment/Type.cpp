#include "Environment/Type.hpp"
#include "Environment/LiteralInteger.hpp"
#include "Environment/LiteralPositiveInteger.hpp"
#include "Environment/LiteralNegativeInteger.hpp"
#include "Environment/LiteralCharacter.hpp"
#include "Environment/LiteralFloat.hpp"
#include "Environment/RuntimeContext.hpp"
#include "UnitTest++/UnitTest++.h"

using namespace Sysmel::Environment;

SUITE(Type)
{
    TEST(RankToMatchType)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            CHECK_EQUAL(0, LiteralPositiveInteger::__staticType__()->rankToMatchType(LiteralPositiveInteger::__staticType__()));
            CHECK_EQUAL(1, LiteralInteger::__staticType__()->rankToMatchType(LiteralPositiveInteger::__staticType__()));
            CHECK_EQUAL(2, LiteralInteger::__staticType__()->rankToMatchType(LiteralCharacter::__staticType__()));
            CHECK_EQUAL(0, LiteralCharacter::__staticType__()->rankToMatchType(LiteralCharacter::__staticType__()));
            CHECK_EQUAL(-1, LiteralCharacter::__staticType__()->rankToMatchType(LiteralInteger::__staticType__()));

        });
    }
}
