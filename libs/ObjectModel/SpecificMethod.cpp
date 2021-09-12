#include "sysmel/ObjectModel/SpecificMethod.hpp"
#include "sysmel/ObjectModel/Type.hpp"
#include "sysmel/ObjectModel/MacroInvocationContext.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include <iostream>

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<SpecificMethod> specificMethodTypeRegistration;

bool SpecificMethod::isSpecificMethod() const
{
    return true;
}

MethodPatternMatchingResult SpecificMethod::matchPatternForRunWithIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver)
{
    (void)selector;
    // Make sure the argument count matches.
    if(arguments.size() != signature.argumentTypes.size())
        return MethodPatternMatchingResult{};

    PatternMatchingRank totalRank = 0;

    // TODO: Handle the void receiver type special case.
    totalRank = signature.receiverType->rankToMatchValue(receiver);
    if(totalRank < 0)
        return MethodPatternMatchingResult{};

    for(size_t i = 0; i < arguments.size(); ++i)
    {
        auto argumentRank = signature.argumentTypes[i]->rankToMatchValue(arguments[i]);
        if(argumentRank < 0)
            return MethodPatternMatchingResult{};
        totalRank += argumentRank;
    }

    return MethodPatternMatchingResult{shared_from_this(), totalRank};
}

bool SpecificMethod::isMacroMethod() const
{
    return signature.receiverType && signature.receiverType->isKindOf(MacroInvocationContext::__staticType__());
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius