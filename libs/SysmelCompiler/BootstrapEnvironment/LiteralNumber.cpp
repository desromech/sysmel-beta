#include "sysmel/BootstrapEnvironment/LiteralNumber.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/SubclassResponsibility.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<LiteralNumber> literalNumberTypeRegistration;

MethodCategories LiteralNumber::__instanceMethods__()
{
    return MethodCategories{
        {"arithmetic", {
            // Negation
            makeMethodBinding<LiteralNumberPtr (LiteralNumberPtr)> ("negated", +[](const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return LiteralNumberPtr();
            }),
            makeMethodBinding<LiteralNumberPtr (LiteralNumberPtr)> ("pre--", +[](const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return LiteralNumberPtr();
            }),

            // Addition
            makeMethodBinding<LiteralNumberPtr (LiteralNumberPtr, LiteralNumberPtr)> ("+", +[](const LiteralNumberPtr &, const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return LiteralNumberPtr();
            }),

            // Subtraction
            makeMethodBinding<LiteralNumberPtr (LiteralNumberPtr, LiteralNumberPtr)> ("-", +[](const LiteralNumberPtr &, const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return LiteralNumberPtr();
            }),

            // Multiplication
            makeMethodBinding<LiteralNumberPtr (LiteralNumberPtr, LiteralNumberPtr)> ("*", +[](const LiteralNumberPtr &, const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return LiteralNumberPtr();
            }),

            // Division
            makeMethodBinding<LiteralNumberPtr (LiteralNumberPtr, LiteralNumberPtr)> ("/", +[](const LiteralNumberPtr &, const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return LiteralNumberPtr();
            }),
        }},

        {"converting", {
            makeMethodBinding("asFloat", &LiteralNumber::asFloat),
        }}
    };
}

bool LiteralNumber::isLiteralNumber() const
{
    return true;
}

double LiteralNumber::asFloat() const
{
    SysmelSelfSubclassResponsibility();
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius