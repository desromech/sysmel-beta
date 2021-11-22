#include "sysmel/BootstrapEnvironment/TemporaryReferenceType.hpp"
#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<TemporaryReferenceTypeValue> TemporaryReferenceTypeValueTypeRegistration;

TemporaryReferenceTypePtr TemporaryReferenceType::make(const TypePtr &baseType)
{
    return makeWithAddressSpace(baseType, internSymbol("generic"));
}

TemporaryReferenceTypePtr TemporaryReferenceType::makeWithAddressSpace(const TypePtr &baseType, const AnyValuePtr &addressSpace)
{
    auto &cache = RuntimeContext::getActive()->temporaryReferenceTypeCache;
    auto it = cache.find({baseType, addressSpace});
    if(it != cache.end())
        return it->second;

    auto result = std::make_shared<TemporaryReferenceType> ();
    result->setSupertypeAndImplicitMetaType(TemporaryReferenceTypeValue::__staticType__());
    result->baseType = baseType;
    result->addressSpace = addressSpace;
    cache.insert({{baseType, addressSpace}, result});
    return result;
}

bool TemporaryReferenceType::isTemporaryReferenceType() const
{
    return true;
}

ReferenceTypePtr TemporaryReferenceType::ref()
{
    return baseType->ref();
}

ReferenceTypePtr TemporaryReferenceType::refFor(const AnyValuePtr &newAddressSpace)
{
    return baseType->refFor(newAddressSpace);
}

PointerLikeTypePtr TemporaryReferenceType::tempRef()
{
    return shared_from_this();
}

PointerLikeTypePtr TemporaryReferenceType::tempRefFor(const AnyValuePtr &newAddressSpace)
{
    if(addressSpace == newAddressSpace)
        return shared_from_this();

    return baseType->tempRefFor(newAddressSpace);
}

bool TemporaryReferenceTypeValue::isTemporaryReferenceTypeValue() const
{
    return true;
}

TypePtr TemporaryReferenceTypeValue::getType() const
{
    return type;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius