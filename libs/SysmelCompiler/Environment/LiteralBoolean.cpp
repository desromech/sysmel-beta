#include "Environment/LiteralBoolean.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include "Environment/ASTAnalysisEnvironment.hpp"
#include "Environment/ASTBuilder.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/ASTExplicitCastNode.hpp"
#include "Environment/PrimitiveBooleanType.hpp"
#include "Environment/MacroInvocationContext.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<LiteralBoolean> literalBooleanRegistration;

TypePtr WrapperTypeFor<bool>::apply()
{
    return LiteralBoolean::__staticType__();
}

MethodCategories LiteralBoolean::__instanceMethods__()
{
    return MethodCategories{
        {"boolean algebra", {
            makeMethodBinding<bool (bool)> ("not", [](bool self) {
                return !self;
            }, MethodFlags::Pure),
        }}
    };
}


MethodCategories LiteralBoolean::__instanceMacroMethods__()
{
    return MethodCategories{
        {"primitive literal suffixes", {
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("b8", [](const MacroInvocationContextPtr &macroContext) {
                return macroContext->astBuilder->explicitCastTo(
                    macroContext->receiverNode,
                    macroContext->astBuilder->literal(Boolean8::__staticType__())
                );
            }, MethodFlags::Macro),
        }}
    };
}

TypePtr LiteralBoolean::__asInferredTypeForWithModeInEnvironment__(const TypePtr &selfType, const ASTNodePtr &node, TypeInferenceMode mode, bool isMutable, bool concreteLiterals, const ASTAnalysisEnvironmentPtr &environment)
{
    if(concreteLiterals && node->isASTLiteralValueNode() && !environment->hasValidLiteralValueInferrenceType())
        return Boolean8::__staticType__();

    return SuperType::__asInferredTypeForWithModeInEnvironment__(selfType, node, mode, isMutable, concreteLiterals, environment);
}

bool LiteralBoolean::isLiteralBoolean() const
{
    return true;
}

AnyValuePtr LiteralBoolean::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitLiteralBoolean(selfFromThis());
}

} // End of namespace Environment
} // End of namespace Sysmel