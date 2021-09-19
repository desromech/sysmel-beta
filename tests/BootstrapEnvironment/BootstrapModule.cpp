#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapModule.hpp"

#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapType.hpp"

#include "sysmel/BootstrapEnvironment/LiteralValue.hpp"
#include "sysmel/BootstrapEnvironment/LiteralNumber.hpp"
#include "sysmel/BootstrapEnvironment/LiteralInteger.hpp"
#include "sysmel/BootstrapEnvironment/LiteralPositiveInteger.hpp"
#include "sysmel/BootstrapEnvironment/LiteralNegativeInteger.hpp"
#include "sysmel/BootstrapEnvironment/LiteralCharacter.hpp"
#include "sysmel/BootstrapEnvironment/LiteralFloat.hpp"
#include "sysmel/BootstrapEnvironment/LiteralString.hpp"
#include "sysmel/BootstrapEnvironment/LiteralSymbol.hpp"

#include "UnitTest++/UnitTest++.h"

using namespace SysmelMoebius::BootstrapEnvironment;

SUITE(BootstrapModule)
{
    TEST(Default)
    {
        CHECK(RuntimeContext::create()->getBootstrapModule());
    }

    TEST(CoreTypes)
    {
        auto context = RuntimeContext::create();
        context->activeDuring([&](){
            auto module = context->getBootstrapModule();

            CHECK(module->getBootstrapDefinedTypeNamed("AnyValue"));
            CHECK_EQUAL(module->getBootstrapDefinedTypeNamed("AnyValue"), AnyValue::__staticType__());

            // Literals
            CHECK(module->getBootstrapDefinedTypeNamed("LiteralValue"));
            CHECK_EQUAL(module->getBootstrapDefinedTypeNamed("LiteralValue"), LiteralValue::__staticType__());

            CHECK(module->getBootstrapDefinedTypeNamed("LiteralNumber"));
            CHECK_EQUAL(module->getBootstrapDefinedTypeNamed("LiteralNumber"), LiteralNumber::__staticType__());

            CHECK(module->getBootstrapDefinedTypeNamed("LiteralInteger"));
            CHECK_EQUAL(module->getBootstrapDefinedTypeNamed("LiteralInteger"), LiteralInteger::__staticType__());

            CHECK(module->getBootstrapDefinedTypeNamed("LiteralPositiveInteger"));
            CHECK_EQUAL(module->getBootstrapDefinedTypeNamed("LiteralPositiveInteger"), LiteralPositiveInteger::__staticType__());

            CHECK(module->getBootstrapDefinedTypeNamed("LiteralNegativeInteger"));
            CHECK_EQUAL(module->getBootstrapDefinedTypeNamed("LiteralNegativeInteger"), LiteralNegativeInteger::__staticType__());

            CHECK(module->getBootstrapDefinedTypeNamed("LiteralCharacter"));
            CHECK_EQUAL(module->getBootstrapDefinedTypeNamed("LiteralCharacter"), LiteralCharacter::__staticType__());

            CHECK(module->getBootstrapDefinedTypeNamed("LiteralFloat"));
            CHECK_EQUAL(module->getBootstrapDefinedTypeNamed("LiteralFloat"), LiteralFloat::__staticType__());

            CHECK(module->getBootstrapDefinedTypeNamed("LiteralString"));
            CHECK_EQUAL(module->getBootstrapDefinedTypeNamed("LiteralString"), LiteralString::__staticType__());

            CHECK(module->getBootstrapDefinedTypeNamed("LiteralSymbol"));
            CHECK_EQUAL(module->getBootstrapDefinedTypeNamed("LiteralSymbol"), LiteralSymbol::__staticType__());

            // Program entities
            CHECK(module->getBootstrapDefinedTypeNamed("ProgramEntity"));
            CHECK_EQUAL(module->getBootstrapDefinedTypeNamed("ProgramEntity"), ProgramEntity::__staticType__());

            CHECK(module->getBootstrapDefinedTypeNamed("Module"));
            CHECK_EQUAL(module->getBootstrapDefinedTypeNamed("Module"), Module::__staticType__());

            CHECK(module->getBootstrapDefinedTypeNamed("BootstrapModule"));
            CHECK_EQUAL(module->getBootstrapDefinedTypeNamed("BootstrapModule"), BootstrapModule::__staticType__());

            CHECK(module->getBootstrapDefinedTypeNamed("RuntimeContext"));
            CHECK_EQUAL(module->getBootstrapDefinedTypeNamed("RuntimeContext"), RuntimeContext::__staticType__());

            CHECK(module->getBootstrapDefinedTypeNamed("Type"));
            CHECK_EQUAL(module->getBootstrapDefinedTypeNamed("Type"), Type::__staticType__());

            CHECK(module->getBootstrapDefinedTypeNamed("BootstrapType"));
            CHECK_EQUAL(module->getBootstrapDefinedTypeNamed("BootstrapType"), BootstrapType::__staticType__());
        });
    }
}
