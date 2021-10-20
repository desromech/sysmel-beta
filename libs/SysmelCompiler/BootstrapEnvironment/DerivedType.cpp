#include "sysmel/BootstrapEnvironment/DerivedType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<DerivedTypeValue> DerivedTypeValueTypeRegistration;

bool DerivedType::isDerivedType() const
{
    return true;
}

const TypePtr &DerivedType::getBaseType()
{
    return baseType;
}

void DerivedType::setBaseType(const TypePtr &newBaseType)
{
    baseType = newBaseType;
}

bool DerivedTypeValue::isDerivedTypeValue() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius