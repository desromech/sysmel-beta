#include "sysmel/BootstrapEnvironment/ReferenceType.hpp"
#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ReferenceTypeValue> ReferenceTypeValueTypeRegistration;

ReferenceTypePtr ReferenceType::make(const TypePtr &baseType)
{
    return makeWithAddressSpace(baseType, internSymbol("generic"));
}

ReferenceTypePtr ReferenceType::makeWithAddressSpace(const TypePtr &baseType, const AnyValuePtr &addressSpace)
{
    auto &cache = RuntimeContext::getActive()->referenceTypeCache;
    auto it = cache.find({baseType, addressSpace});
    if(it != cache.end())
        return it->second;

    auto result = std::make_shared<ReferenceType> ();
    result->setSupertypeAndImplicitMetaType(ReferenceTypeValue::__staticType__());
    result->baseType = baseType;
    result->addressSpace = addressSpace;
    cache.insert({{baseType, addressSpace}, result});
    return result;
}

bool ReferenceType::isReferenceType() const
{
    return true;
}

bool ReferenceType::isReferenceLikeType() const
{
    return true;
}

ReferenceTypePtr ReferenceType::ref()
{
    return shared_from_this();
}

ReferenceTypePtr ReferenceType::refFor(const AnyValuePtr &newAddressSpace)
{
    if(addressSpace == newAddressSpace)
        return shared_from_this();

    return baseType->refFor(newAddressSpace);
}

PointerLikeTypePtr ReferenceType::tempRef()
{
    return shared_from_this();
}

PointerLikeTypePtr ReferenceType::tempRefFor(const AnyValuePtr &newAddressSpace)
{
    if(addressSpace == newAddressSpace)
        return shared_from_this();

    return baseType->refFor(newAddressSpace);
}

std::string ReferenceType::printString() const
{
    if(hasGenericAddressSpace())
        return baseType->printString() + " ref";
    return "(" + baseType->printString() + " refFor: " + addressSpace->printString();
}

SExpression ReferenceType::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"refType"}},
        baseType->asSExpression(),
        addressSpace->asSExpression()
    }};
}

PointerLikeTypeValuePtr ReferenceType::makeWithValue(const AnyValuePtr &value)
{
    auto reference = std::make_shared<ReferenceTypeValue> ();
    reference->type = shared_from_this();
    reference->baseValue = value;
    return reference;
}

bool ReferenceTypeValue::isReferenceTypeValue() const
{
    return true;
}

bool ReferenceTypeValue::isReferenceLikeTypeValue() const
{
    return true;
}

TypePtr ReferenceTypeValue::getType() const
{
    return type;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius