#include "sysmel/BootstrapEnvironment/MethodDictionary.hpp"
#include "sysmel/BootstrapEnvironment/PatternMatchingMethod.hpp"
#include "sysmel/BootstrapEnvironment/CannotOverloadPatternMatchingMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"


namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

void MethodDictionary::addMethodWithSelector(const AnyValuePtr &selector, const AnyValuePtr &method)
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
            auto newPatternMatchingMethod = std::make_shared<PatternMatchingMethod> (selector);
            newPatternMatchingMethod->addPattern(std::static_pointer_cast<Method> (existent));
            newPatternMatchingMethod->addPattern(std::static_pointer_cast<Method> (method));
            newPatternMatchingMethod->registerInCurrentModule();
            dictionary[selector] = newPatternMatchingMethod;
        }
        else
        {
            auto patternMatchingMethod = std::static_pointer_cast<PatternMatchingMethod> (existent);
            patternMatchingMethod->addPattern(std::static_pointer_cast<Method> (method));
        }
        return;
    }

    signalNew<CannotOverloadPatternMatchingMethod> ();
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius