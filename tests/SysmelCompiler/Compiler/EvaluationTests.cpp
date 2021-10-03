#include "sysmel/Compiler/Sysmel/SysmelLanguageSupport.hpp"
#include "sysmel/BootstrapEnvironment/AnyValue.hpp"
#include "sysmel/BootstrapEnvironment/Wrappers.hpp"
#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapModule.hpp"
#include "sysmel/BootstrapEnvironment/ScriptModule.hpp"

#include "UnitTest++/UnitTest++.h"

using namespace SysmelMoebius::BootstrapEnvironment;

static AnyValuePtr evaluateString(const std::string &source)
{
    return SysmelLanguageSupport::uniqueInstance()->evaluateSourceStringNamed(source, "unit-test");
}

template<typename T>
static T evaluateStringWithValueOfType(const std::string &source)
{
    return unwrapValue<T> (evaluateString(source));
}

SUITE(SysmelCompileTimeEvaluation)
{
    TEST(Literals)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int> ("0"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int> ("1"));
                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int> ("-1"));

                CHECK_EQUAL(true, evaluateStringWithValueOfType<bool> ("true"));
                CHECK_EQUAL(false, evaluateStringWithValueOfType<bool> ("false"));
                CHECK(evaluateString("void")->isVoid());
                CHECK(evaluateString("nil")->isUndefined());
            });
        });
    }

    TEST(Arithmetic)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK_EQUAL(2, evaluateStringWithValueOfType<int> ("1 + 1"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int> ("1 - 1"));
                CHECK_EQUAL(2, evaluateStringWithValueOfType<int> ("1+1"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int> ("1-1"));

                CHECK_EQUAL(6, evaluateStringWithValueOfType<int> ("2*3"));
                CHECK_EQUAL(5.5, evaluateStringWithValueOfType<double> ("2.5 + 3"));
                CHECK_EQUAL(5.5, evaluateStringWithValueOfType<double> ("2.5+3"));

                CHECK_EQUAL(Fraction({1}, {2}), evaluateStringWithValueOfType<Fraction> ("1/2"));
                CHECK_EQUAL(Fraction({3}, {2}), evaluateStringWithValueOfType<Fraction> ("1 + 1/2"));
                CHECK_EQUAL(Fraction({3}, {2}), evaluateStringWithValueOfType<Fraction> ("1/2 + 1"));
            });
        });
    }

    TEST(LocalVariable)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK_EQUAL(42, evaluateStringWithValueOfType<int> ("let a := 42. a"));
            });
        });
    }
}
