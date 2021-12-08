#include "Environment/SSAConstantValue.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSAConstantValue> SSAConstantValueTypeRegistration;

bool SSAConstantValue::isSSAConstantValue() const
{
    return true;
}

} // End of namespace Environment
} // End of namespace Sysmel