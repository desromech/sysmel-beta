#include "sysmel/ObjectModel/LiteralNumber.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include "sysmel/ObjectModel/Error.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace ObjectModel
{
static BootstrapTypeRegistration<LiteralNumber> literalNumberTypeRegistration;

MethodCategories LiteralNumber::__instanceMethods__()
{
    return MethodCategories{
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
    throw SubclassResponsibility();
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius