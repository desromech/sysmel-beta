#include "Environment/PrimitiveScalarType.hpp"
#include "Environment/PrimitiveVectorType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include <algorithm>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<PrimitiveScalarType> PrimitiveScalarTypeTypeRegistration;

MethodCategories PrimitiveScalarType::__typeMethods__()
{
    return MethodCategories{
        {"composing", {
            makeMethodBinding<TypePtr (TypePtr, uint32_t)> ("primitiveVectorWithElements:", [](const TypePtr &elementType, uint32_t elements){
                return PrimitiveVectorType::make(elementType, elements);
            }, MethodFlags::Pure),
        }},
    };
}

bool PrimitiveScalarType::isPrimitiveScalarTypeValue() const
{
    return true;
}

} // End of namespace Environment
} // End of namespace Sysmel