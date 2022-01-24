#include "Environment/MethodDictionary.hpp"
#include "Environment/PatternMatchingMethod.hpp"
#include "Environment/CannotOverloadPatternMatchingMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"


namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<MethodDictionary> methodDictionaryTypeRegistration;

bool MethodDictionary::isMethodDictionary() const
{
    return true;
}

AnyValuePtr MethodDictionary::lookupSelector(const AnyValuePtr &selector)
{
    auto it = dictionary.find(selector);
    if(it != dictionary.end())
        return it->second;

    return AnyValuePtr();
}

void MethodDictionary::addMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector)
{
    auto it = dictionary.find(selector);
    if(it == dictionary.end())
    {
        dictionary.insert(std::make_pair(selector, method));
        return;
    }

    if(method->isPatternMatchingMethod())
        signalNew<CannotOverloadPatternMatchingMethod> ();

    auto existent = it->second;
    if(method->isMethod() && existent->isMethod())
    {
        if(!existent->isPatternMatchingMethod())
        {
            auto newPatternMatchingMethod = basicMakeObject<PatternMatchingMethod> (selector);
            newPatternMatchingMethod->addPattern(staticObjectCast<Method> (existent));
            newPatternMatchingMethod->addPattern(staticObjectCast<Method> (method));
            dictionary[selector] = newPatternMatchingMethod;
        }
        else
        {
            auto patternMatchingMethod = staticObjectCast<PatternMatchingMethod> (existent);
            patternMatchingMethod->addPattern(staticObjectCast<Method> (method));
        }
        return;
    }

    signalNew<CannotOverloadPatternMatchingMethod> ();
}

void MethodDictionary::replaceMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector)
{
    dictionary[selector] = method;
}

} // End of namespace Environment
} // End of namespace Sysmel