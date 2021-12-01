#include "sysmel/BootstrapEnvironment/SSACodeRegionLocalValue.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSACodeRegionLocalValue> SSACodeRegionLocalValueTypeRegistration;

bool SSACodeRegionLocalValue::isSSACodeRegionLocalValue() const
{
    return true;
}

SExpression SSACodeRegionLocalValue::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"local"}},
        LargeInteger{localValueIndex}
    }};
}

void SSACodeRegionLocalValue::enumerateLocalValues(SSACodeRegionLocalValueEnumerationState &state)
{
    localValueIndex = state.localValueCount++;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius