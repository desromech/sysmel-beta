#include "Environment/ResultTypeInferenceSlot.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ResultTypeInferenceSlot> ResultTypeInferenceSlotTypeRegistration;

ASTNodePtr ResultTypeInferenceSlot::concretizeTypeInferenceOfNodeWith(const ASTNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    (void)semanticAnalyzer;
    return node;
}

TypePtr ResultTypeInferenceSlot::getExpectedFunctionalArgumentType(size_t index) const
{
    (void)index;
    return nullptr;
}

TypePtr ResultTypeInferenceSlot::getExpectedFunctionalResultType() const
{
    return nullptr;
}


} // End of namespace Environment
} // End of namespace Sysmel