#include "sysmel/BootstrapEnvironment/FunctionalType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<FunctionalTypeValue> functionalTypeRegistration;

bool FunctionalType::isFunctionalType() const
{
    return true;
}

ASTNodePtr FunctionalType::analyzeCallNode(const ASTCallNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    assert("TODO: analyzeCallNode" && false);
}

bool FunctionalTypeValue::isFunctionalTypeValue() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius