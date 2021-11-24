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
        {"comparisons", {
            // =
            makeMethodBinding<bool (LiteralNumberPtr, AnyValuePtr)> ("=", +[](const LiteralNumberPtr &, const AnyValuePtr &) {
                SysmelSelfSubclassResponsibility();
                return false;
            }, MethodFlags::Pure | MethodFlags::Abstract),
            // ~=
            makeMethodBinding<bool (LiteralNumberPtr, AnyValuePtr)> ("~=", +[](const LiteralNumberPtr &, const AnyValuePtr &) {
                SysmelSelfSubclassResponsibility();
                return false;
            }, MethodFlags::Pure | MethodFlags::Abstract),

            // <
            makeMethodBinding<bool (LiteralNumberPtr, LiteralNumberPtr)> ("<", +[](const LiteralNumberPtr &, const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return false;
            }, MethodFlags::Pure | MethodFlags::Abstract),

            // <=
            makeMethodBinding<bool (LiteralNumberPtr, LiteralNumberPtr)> ("<=", +[](const LiteralNumberPtr &, const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return false;
            }, MethodFlags::Pure | MethodFlags::Abstract),

            // >
            makeMethodBinding<bool (LiteralNumberPtr, LiteralNumberPtr)> (">", +[](const LiteralNumberPtr &, const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return false;
            }, MethodFlags::Pure | MethodFlags::Abstract),

            // >=
            makeMethodBinding<bool (LiteralNumberPtr, LiteralNumberPtr)> (">=", +[](const LiteralNumberPtr &, const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return false;
            }, MethodFlags::Pure | MethodFlags::Abstract),
        }},

        {"arithmetic", {
            // Negation
            makeMethodBinding<LiteralNumberPtr (LiteralNumberPtr)> ("negated", +[](const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return LiteralNumberPtr();
            }, MethodFlags::Pure | MethodFlags::Abstract),
            makeMethodBinding<LiteralNumberPtr (LiteralNumberPtr)> ("pre--", +[](const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return LiteralNumberPtr();
            }, MethodFlags::Pure | MethodFlags::Abstract),

            // Addition
            makeMethodBinding<LiteralNumberPtr (LiteralNumberPtr, LiteralNumberPtr)> ("+", +[](const LiteralNumberPtr &, const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return LiteralNumberPtr();
            }, MethodFlags::Pure | MethodFlags::Abstract),

            // Subtraction
            makeMethodBinding<LiteralNumberPtr (LiteralNumberPtr, LiteralNumberPtr)> ("-", +[](const LiteralNumberPtr &, const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return LiteralNumberPtr();
            }, MethodFlags::Pure | MethodFlags::Abstract),

            // Multiplication
            makeMethodBinding<LiteralNumberPtr (LiteralNumberPtr, LiteralNumberPtr)> ("*", +[](const LiteralNumberPtr &, const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return LiteralNumberPtr();
            }, MethodFlags::Pure | MethodFlags::Abstract),

            // Division
            makeMethodBinding<LiteralNumberPtr (LiteralNumberPtr, LiteralNumberPtr)> ("/", +[](const LiteralNumberPtr &, const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return LiteralNumberPtr();
            }, MethodFlags::Pure | MethodFlags::Abstract),
        }},

        {"converting", {
            makeMethodBinding("asFloat", &LiteralNumber::asFloat, MethodFlags::Pure | MethodFlags::Abstract),
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