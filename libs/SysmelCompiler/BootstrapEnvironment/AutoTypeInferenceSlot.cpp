#include "sysmel/BootstrapEnvironment/AutoTypeInferenceSlot.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticAnalyzer.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<AutoTypeInferenceSlot> AutoTypeInferenceSlotTypeRegistration;

ResultTypeInferenceSlotPtr ResultTypeInferenceSlot::makeForAuto()
{
    return basicMakeObject<AutoTypeInferenceSlot> ();
}

ResultTypeInferenceSlotPtr ResultTypeInferenceSlot::makeForTemporaryAuto()
{
    auto result = basicMakeObject<AutoTypeInferenceSlot> ();
    result->mode = TypeInferenceMode::TemporaryReference;
    return result;
}

ResultTypeInferenceSlotPtr ResultTypeInferenceSlot::makeForAutoWithMode(TypeInferenceMode mode, bool isMutable, bool concreteLiterals)
{
    auto result = basicMakeObject<AutoTypeInferenceSlot> ();
    result->mode = mode;
    result->isMutable = isMutable;
    result->concreteLiterals = concreteLiterals;
    return result;
}

ASTNodePtr AutoTypeInferenceSlot::concretizeTypeInferenceOfNodeWith(const ASTNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    auto inferredType = node->analyzedType->asInferredTypeForWithModeInEnvironment(node, mode, isMutable, concreteLiterals, semanticAnalyzer->environment);
    if(inferredType == node->analyzedType)
        return node;

    if(!inferredType)
        return semanticAnalyzer->recordSemanticErrorInNode(node, "Cannot perform type inference with the selected mode.");

    return semanticAnalyzer->addImplicitCastTo(node, inferredType);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius