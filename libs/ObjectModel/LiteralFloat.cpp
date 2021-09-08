#include "sysmel/ObjectModel/LiteralFloat.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include "sysmel/ObjectModel/Error.hpp"
#include <algorithm>
#include <sstream>

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<LiteralFloat> literalFloatTypeRegistration;

TypePtr WrapperTypeFor<double>::apply()
{
    return LiteralFloat::__staticType__();
}

MethodCategories LiteralFloat::__instanceMethods__()
{
    return MethodCategories{
        {"arithmetic", {
            makeMethodBinding<double (double)> ("negated", +[](double value) {
                return -value;
            })
        }}
    };
}

bool LiteralFloat::isLiteralFloat() const
{
    return true;
}

std::string LiteralFloat::printString() const
{
    std::ostringstream out;
    out << value;
    return out.str();
}

float LiteralFloat::unwrapAsFloat32() const
{
    if(float(value) != value)
        throw CannotUnwrap("Cannot unwrap float32 because of loss of precision.");
    return value;
}

double LiteralFloat::unwrapAsFloat64() const
{
    return value;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius