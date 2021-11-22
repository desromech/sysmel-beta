#include "sysmel/BootstrapEnvironment/PointerType.hpp"
#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<PointerTypeValue> PointerTypeValueTypeRegistration;

PointerTypePtr PointerType::make(const TypePtr &baseType)
{
    return makeWithAddressSpace(baseType, internSymbol("generic"));
}

PointerTypePtr PointerType::makeWithAddressSpace(const TypePtr &baseType, const AnyValuePtr &addressSpace)
{
    auto &cache = RuntimeContext::getActive()->pointerTypeCache;
    auto it = cache.find({baseType, addressSpace});
    if(it != cache.end())
        return it->second;

    auto result = std::make_shared<PointerType> ();
    result->setSupertypeAndImplicitMetaType(PointerTypeValue::__staticType__());
    result->baseType = baseType;
    result->addressSpace = addressSpace;
    cache.insert({{baseType, addressSpace}, result});
    return result;
}

bool PointerType::isPointerType() const
{
    return true;
}
bool PointerTypeValue::isPointerTypeValue() const
{
    return true;
}

TypePtr PointerTypeValue::getType() const
{
    return type;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius