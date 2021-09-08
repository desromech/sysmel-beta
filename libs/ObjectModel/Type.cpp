#include "sysmel/ObjectModel/Type.hpp"
#include "sysmel/ObjectModel/Error.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include "sysmel/ObjectModel/PatternMatchingMethod.hpp"
#include <algorithm>
#include <iostream>

namespace SysmelMoebius
{
namespace ObjectModel
{
static BootstrapTypeRegistration<Type> typeTypeRegistration;

TypePtr Type::getSuperType()
{
    return supertype;
}

AnyValuePtr Type::lookupLocalSelector(const AnyValuePtr &selector)
{
    auto it = methodDictionary.find(selector);
    if(it != methodDictionary.end())
        return it->second;

    return AnyValuePtr();
}

AnyValuePtr Type::lookupSelector(const AnyValuePtr &selector)
{
    auto localMethod = lookupLocalSelector(selector);
    if(localMethod)
        return localMethod;

    auto superType = getSuperType();
    if(superType)
        return superType->lookupSelector(selector);

    return AnyValuePtr();
}

void Type::addMethodWithSelector(const AnyValuePtr &selector, const AnyValuePtr &method)
{
    auto it = methodDictionary.find(selector);
    if(it == methodDictionary.end())
    {
        methodDictionary.insert(std::make_pair(selector, method));
        return;
    }

    if(method->isPatternMatchingMethod())
        throw CannotOverloadPatternMatchingMethod();

    auto existent = it->second;
    if(method->isMethod() && existent->isMethod())
    {
        if(!existent->isPatternMatchingMethod())
        {
            auto newPatternMatchingMethod = std::make_shared<PatternMatchingMethod> (selector);
            newPatternMatchingMethod->addPattern(std::static_pointer_cast<Method> (existent));
            newPatternMatchingMethod->addPattern(std::static_pointer_cast<Method> (method));
            methodDictionary[selector] = newPatternMatchingMethod;
        }
        else
        {
            auto patternMatchingMethod = std::static_pointer_cast<PatternMatchingMethod> (existent);
            patternMatchingMethod->addPattern(std::static_pointer_cast<Method> (method));
        }
        return;
    }

    throw CannotOverloadPatternMatchingMethod();
}

PatternMatchingRank Type::rankToMatchType(const TypePtr &type)
{
    PatternMatchingRank rank = 0;
    for(auto currentType = type; currentType; currentType = currentType->getSuperType())
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

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius