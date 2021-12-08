#include "Environment/DerivedType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel