#include "Environment/DerivedType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
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

MethodCategories DerivedTypeValue::__typeMethods__()
{
    return MethodCategories{
        {"accessing", {
            makeMethodBinding<TypePtr (TypePtr)> ("baseType", [](const TypePtr &type) -> TypePtr {
                return type->isDerivedType() ? type.staticAs<DerivedType> ()->getBaseType() : Type::getVoidType();
            }, MethodFlags::Pure),
        }}
    };
}


bool DerivedTypeValue::isDerivedTypeValue() const
{
    return true;
}

} // End of namespace Environment
} // End of namespace Sysmel