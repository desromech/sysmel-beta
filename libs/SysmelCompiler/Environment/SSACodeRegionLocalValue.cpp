#include "Environment/SSACodeRegionLocalValue.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel