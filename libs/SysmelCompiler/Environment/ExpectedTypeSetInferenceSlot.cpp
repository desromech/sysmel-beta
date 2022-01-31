#include "Environment/ExpectedTypeSetInferenceSlot.hpp"
#include "Environment/Type.hpp"
#include "Environment/FunctionalType.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ExpectedTypeSetInferenceSlot> ExpectedTypeSetInferenceSlotTypeRegistration;

ResultTypeInferenceSlotPtr ResultTypeInferenceSlot::makeForType(const TypePtr &expectedType)
{
    if(expectedType->isAutoType())
        return makeForAuto();
    return makeForTypeSet({expectedType});
}

ResultTypeInferenceSlotPtr ResultTypeInferenceSlot::makeForReceiverType(const TypePtr &expectedType)
{
    if(expectedType->isAutoType())
        return makeForAuto();
    auto typeInferenceSlot = basicMakeObject<ExpectedTypeSetInferenceSlot> ();
    typeInferenceSlot->expectedTypeSet = {expectedType};
    typeInferenceSlot->isReceiverType = true;
    return typeInferenceSlot;
}

ResultTypeInferenceSlotPtr ResultTypeInferenceSlot::makeForTypeSet(const TypePtrList &expectedTypeSet)
{
    sysmelAssert(!expectedTypeSet.empty());
    auto typeInferenceSlot = basicMakeObject<ExpectedTypeSetInferenceSlot> ();
    typeInferenceSlot->expectedTypeSet = expectedTypeSet;
    return typeInferenceSlot;
}

ASTNodePtr ExpectedTypeSetInferenceSlot::concretizeTypeInferenceOfNodeWith(const ASTNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    return semanticAnalyzer->addImplicitCastToOneOf(node, expectedTypeSet, isReceiverType);
}

TypePtr ExpectedTypeSetInferenceSlot::getExpectedFunctionalArgumentType(size_t index) const
{
    TypePtr argumentType;
    for(auto &type : expectedTypeSet)
    {
        if(type->isFunctionalType() && index < type.staticAs<FunctionalType> ()->getArgumentCount())
        {
            auto newArgumentType = type.staticAs<FunctionalType> ()->getArgument(index);
            if(argumentType && argumentType != newArgumentType)
                return nullptr;
            
            argumentType = newArgumentType;
        }
    }

    return argumentType;
}

TypePtr ExpectedTypeSetInferenceSlot::getExpectedFunctionalResultType() const
{
    TypePtr resultType;
    for(auto &type : expectedTypeSet)
    {
        if(type->isFunctionalType())
        {
            auto newResultType = type.staticAs<FunctionalType> ()->getResultType();
            if(resultType && resultType != newResultType)
                return nullptr;
            
            resultType = newResultType;
        }
    }

    return resultType;
}

} // End of namespace Environment
} // End of namespace Sysmel