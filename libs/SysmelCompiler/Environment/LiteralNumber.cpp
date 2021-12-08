#include "Environment/LiteralNumber.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/SubclassResponsibility.hpp"
#include <algorithm>

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel