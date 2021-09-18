#include "sysmel/ObjectModel/Type.hpp"
#include "sysmel/ObjectModel/MethodDictionary.hpp"
#include "sysmel/ObjectModel/Error.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace ObjectModel
{
static BootstrapTypeRegistration<Type> typeTypeRegistration;

MethodCategories Type::__instanceMethods__()
{
    return MethodCategories{
        {"accessing", {
            makeMethodBinding("supertype", &Type::getSupertype),
            makeMethodBinding("subtypes", &Type::getSubtypes),
        }},
    };
}

bool Type::isType() const
{
    return true;
}

TypePtr Type::getSupertype()
{
    return supertype;
}

void Type::setSupertype(const TypePtr &newSupertype)
{
    supertype = newSupertype;
}

const TypePtrList &Type::getSubtypes()
{
    return subtypes;
}

void Type::registerSubtype(const TypePtr &subtype)
{
    if(!subtype->isMetaType())
        subtypes.push_back(subtype);
}

AnyValuePtr Type::lookupLocalSelector(const AnyValuePtr &selector)
{
    return methodDictionary ? methodDictionary->lookupSelector(selector) : AnyValuePtr();
}

AnyValuePtr Type::lookupSelector(const AnyValuePtr &selector)
{
    auto localMethod = lookupLocalSelector(selector);
    if(localMethod)
        return localMethod;

    auto superType = getSupertype();
    if(superType)
        return superType->lookupSelector(selector);

    return AnyValuePtr();
}

void Type::addMacroMethodWithSelector(const AnyValuePtr &selector, const AnyValuePtr &method)
{
    if(!macroMethodDictionary)
        macroMethodDictionary = std::make_shared<MethodDictionary> ();
    macroMethodDictionary->addMethodWithSelector(selector, method);
}

void Type::addMacroMethodCategories(const MethodCategories &categories)
{
    for(auto &[category, methods] : categories)
    {
        for(auto &[selector, method] : methods)
            addMacroMethodWithSelector(selector, method);
    }
}

void Type::addMethodWithSelector(const AnyValuePtr &selector, const AnyValuePtr &method)
{
    if(!methodDictionary)
        methodDictionary = std::make_shared<MethodDictionary> ();
    methodDictionary->addMethodWithSelector(selector, method);
}

void Type::addMethodCategories(const MethodCategories &categories)
{
    for(auto &[category, methods] : categories)
    {
        for(auto &[selector, method] : methods)
            addMethodWithSelector(selector, method);
    }
}

TypePtr Type::getInstanceType()
{
    return shared_from_this();
}

TypePtr Type::getMetaType()
{
    return getType();
}

bool Type::isKindOf(const TypePtr &otherType)
{
    if(this == otherType.get())
        return true;
    
    auto supertype = getSupertype();
    if(supertype)
        return supertype->isKindOf(otherType);

    return false;
}

PatternMatchingRank Type::rankToMatchType(const TypePtr &type)
{
    PatternMatchingRank rank = 0;
    for(auto currentType = type; currentType; currentType = currentType->getSupertype())
    {
        if(currentType.get() == this)
            return rank;

        ++rank;
    }

    return -1;
}

PatternMatchingRank Type::rankToMatchValue(const AnyValuePtr &value)
{
    return rankToMatchType(value->getType());
}

AnyValuePtr Type::runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver)
{
    auto method = lookupSelector(selector);
    if(method)
        return method->runWithArgumentsIn(selector, arguments, receiver);

    throw MessageNotUnderstood("Message " + selector->printString() + " is not understood by " + receiver->printString() + ".");
}

void Type::subtypesDo(const TypeIterationBlock &aBlock)
{
    for(auto &subtype : subtypes)
        aBlock(subtype);
}

void Type::allSubtypesDo(const TypeIterationBlock &aBlock)
{
    TypeIterationBlock recursiveBlock = [&](const TypePtr &type) {
        aBlock(type);
        type->subtypesDo(recursiveBlock);
    };

    subtypesDo(recursiveBlock);
}

void Type::withAllSubtypesDo(const TypeIterationBlock &aBlock)
{
    aBlock(shared_from_this());
    allSubtypesDo(aBlock);
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius