#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/LiteralInteger.hpp"
#include "sysmel/BootstrapEnvironment/LiteralPositiveInteger.hpp"
#include "sysmel/BootstrapEnvironment/LiteralNegativeInteger.hpp"
#include "sysmel/BootstrapEnvironment/LiteralCharacter.hpp"
#include "sysmel/BootstrapEnvironment/LiteralFloat.hpp"
#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"
#include "UnitTest++/UnitTest++.h"

using namespace SysmelMoebius::BootstrapEnvironment;

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
