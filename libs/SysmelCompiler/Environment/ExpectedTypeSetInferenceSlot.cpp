#include "Environment/ExpectedTypeSetInferenceSlot.hpp"
#include "Environment/Type.hpp"
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

} // End of namespace Environment
} // End of namespace Sysmel