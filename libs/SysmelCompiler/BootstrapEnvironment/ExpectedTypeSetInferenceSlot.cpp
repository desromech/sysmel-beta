#include "sysmel/BootstrapEnvironment/ExpectedTypeSetInferenceSlot.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticAnalyzer.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ExpectedTypeSetInferenceSlot> ExpectedTypeSetInferenceSlotTypeRegistration;

ResultTypeInferenceSlotPtr ResultTypeInferenceSlot::makeForType(const TypePtr &expectedType)
{
    if(expectedType->isAutoType())
        return makeForAuto();
    return makeForTypeSet({expectedType});
}

ResultTypeInferenceSlotPtr ResultTypeInferenceSlot::makeForTypeSet(const TypePtrList &expectedTypeSet)
{
    assert(!expectedTypeSet.empty());
    auto typeInferenceSlot = std::make_shared<ExpectedTypeSetInferenceSlot> ();
    typeInferenceSlot->expectedTypeSet = expectedTypeSet;
    return typeInferenceSlot;
}

ASTNodePtr ExpectedTypeSetInferenceSlot::concretizeTypeInferenceOfNodeWith(const ASTNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    return semanticAnalyzer->addImplicitCastToOneOf(node, expectedTypeSet);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius