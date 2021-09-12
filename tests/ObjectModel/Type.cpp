#include "sysmel/ObjectModel/Type.hpp"
#include "sysmel/ObjectModel/LiteralInteger.hpp"
#include "sysmel/ObjectModel/LiteralPositiveInteger.hpp"
#include "sysmel/ObjectModel/LiteralNegativeInteger.hpp"
#include "sysmel/ObjectModel/LiteralCharacter.hpp"
#include "sysmel/ObjectModel/LiteralFloat.hpp"
#include "sysmel/ObjectModel/RuntimeContext.hpp"
#include "UnitTest++/UnitTest++.h"

using namespace SysmelMoebius::ObjectModel;

SUITE(Type)
{
    TEST(RankToMatchType)
    {
        RuntimeContext::create()->activeDuring([&](){
            CHECK_EQUAL(0, LiteralPositiveInteger::__staticType__()->rankToMatchType(LiteralPositiveInteger::__staticType__()));
            CHECK_EQUAL(1, LiteralInteger::__staticType__()->rankToMatchType(LiteralPositiveInteger::__staticType__()));
            CHECK_EQUAL(2, LiteralInteger::__staticType__()->rankToMatchType(LiteralCharacter::__staticType__()));
            CHECK_EQUAL(0, LiteralCharacter::__staticType__()->rankToMatchType(LiteralCharacter::__staticType__()));
            CHECK_EQUAL(-1, LiteralCharacter::__staticType__()->rankToMatchType(LiteralInteger::__staticType__()));

        });
    }
}
