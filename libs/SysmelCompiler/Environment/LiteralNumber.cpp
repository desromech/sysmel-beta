#include "Environment/LiteralNumber.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/MacroInvocationContext.hpp"
#include "Environment/PrimitiveCharacterType.hpp"
#include "Environment/PrimitiveIntegerType.hpp"
#include "Environment/PrimitiveFloatType.hpp"
#include "Environment/ASTAnalysisEnvironment.hpp"
#include "Environment/ASTBuilder.hpp"
#include "Environment/ASTExplicitCastNode.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/SubclassResponsibility.hpp"
#include <algorithm>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<LiteralNumber> literalNumberTypeRegistration;

MethodCategories LiteralNumber::__instanceMacroMethods__()
{
    return MethodCategories{
        {"primitive literal suffixes", {
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("i8", [](const MacroInvocationContextPtr &macroContext) {
                return macroContext->astBuilder->explicitCastTo(
                    macroContext->receiverNode,
                    macroContext->astBuilder->literal(Int8::__staticType__())
                );
            }, MethodFlags::Macro),
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("i16", [](const MacroInvocationContextPtr &macroContext) {
                return macroContext->astBuilder->explicitCastTo(
                    macroContext->receiverNode,
                    macroContext->astBuilder->literal(Int16::__staticType__())
                );
            }, MethodFlags::Macro),
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("i32", [](const MacroInvocationContextPtr &macroContext) {
                return macroContext->astBuilder->explicitCastTo(
                    macroContext->receiverNode,
                    macroContext->astBuilder->literal(Int32::__staticType__())
                );
            }, MethodFlags::Macro),
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("i64", [](const MacroInvocationContextPtr &macroContext) {
                return macroContext->astBuilder->explicitCastTo(
                    macroContext->receiverNode,
                    macroContext->astBuilder->literal(Int64::__staticType__())
                );
            }, MethodFlags::Macro),
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("u8", [](const MacroInvocationContextPtr &macroContext) {
                return macroContext->astBuilder->explicitCastTo(
                    macroContext->receiverNode,
                    macroContext->astBuilder->literal(UInt8::__staticType__())
                );
            }, MethodFlags::Macro),
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("u16", [](const MacroInvocationContextPtr &macroContext) {
                return macroContext->astBuilder->explicitCastTo(
                    macroContext->receiverNode,
                    macroContext->astBuilder->literal(UInt16::__staticType__())
                );
            }, MethodFlags::Macro),
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("u32", [](const MacroInvocationContextPtr &macroContext) {
                return macroContext->astBuilder->explicitCastTo(
                    macroContext->receiverNode,
                    macroContext->astBuilder->literal(UInt32::__staticType__())
                );
            }, MethodFlags::Macro),
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("u64", [](const MacroInvocationContextPtr &macroContext) {
                return macroContext->astBuilder->explicitCastTo(
                    macroContext->receiverNode,
                    macroContext->astBuilder->literal(UInt64::__staticType__())
                );
            }, MethodFlags::Macro),
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("c8", [](const MacroInvocationContextPtr &macroContext) {
                return macroContext->astBuilder->explicitCastTo(
                    macroContext->receiverNode,
                    macroContext->astBuilder->literal(Char8::__staticType__())
                );
            }, MethodFlags::Macro),
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("c16", [](const MacroInvocationContextPtr &macroContext) {
                return macroContext->astBuilder->explicitCastTo(
                    macroContext->receiverNode,
                    macroContext->astBuilder->literal(Char16::__staticType__())
                );
            }, MethodFlags::Macro),
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("c32", [](const MacroInvocationContextPtr &macroContext) {
                return macroContext->astBuilder->explicitCastTo(
                    macroContext->receiverNode,
                    macroContext->astBuilder->literal(Char32::__staticType__())
                );
            }, MethodFlags::Macro),
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("f16", [](const MacroInvocationContextPtr &macroContext) {
                return macroContext->astBuilder->explicitCastTo(
                    macroContext->receiverNode,
                    macroContext->astBuilder->literal(Float16::__staticType__())
                );
            }, MethodFlags::Macro),
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("f32", [](const MacroInvocationContextPtr &macroContext) {
                return macroContext->astBuilder->explicitCastTo(
                    macroContext->receiverNode,
                    macroContext->astBuilder->literal(Float32::__staticType__())
                );
            }, MethodFlags::Macro),
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("f64", [](const MacroInvocationContextPtr &macroContext) {
                return macroContext->astBuilder->explicitCastTo(
                    macroContext->receiverNode,
                    macroContext->astBuilder->literal(Float64::__staticType__())
                );
            }, MethodFlags::Macro),
        }}
    };
}

MethodCategories LiteralNumber::__instanceMethods__()
{
    return MethodCategories{
        {"comparisons", {
            // =
            makeMethodBinding<bool (LiteralNumberPtr, AnyValuePtr)> ("=", +[](const LiteralNumberPtr &, const AnyValuePtr &) {
                SysmelSelfSubclassResponsibility();
                return false;
            }, MethodFlags::Pure | MethodFlags::Abstract),
            // ~=
            makeMethodBinding<bool (LiteralNumberPtr, AnyValuePtr)> ("~=", +[](const LiteralNumberPtr &, const AnyValuePtr &) {
                SysmelSelfSubclassResponsibility();
                return false;
            }, MethodFlags::Pure | MethodFlags::Abstract),

            // <
            makeMethodBinding<bool (LiteralNumberPtr, LiteralNumberPtr)> ("<", +[](const LiteralNumberPtr &, const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return false;
            }, MethodFlags::Pure | MethodFlags::Abstract),

            // <=
            makeMethodBinding<bool (LiteralNumberPtr, LiteralNumberPtr)> ("<=", +[](const LiteralNumberPtr &, const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return false;
            }, MethodFlags::Pure | MethodFlags::Abstract),

            // >
            makeMethodBinding<bool (LiteralNumberPtr, LiteralNumberPtr)> (">", +[](const LiteralNumberPtr &, const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return false;
            }, MethodFlags::Pure | MethodFlags::Abstract),

            // >=
            makeMethodBinding<bool (LiteralNumberPtr, LiteralNumberPtr)> (">=", +[](const LiteralNumberPtr &, const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return false;
            }, MethodFlags::Pure | MethodFlags::Abstract),
        }},

        {"arithmetic", {
            // Negation
            makeMethodBinding<LiteralNumberPtr (LiteralNumberPtr)> ("negated", +[](const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return LiteralNumberPtr();
            }, MethodFlags::Pure | MethodFlags::Abstract),
            makeMethodBinding<LiteralNumberPtr (LiteralNumberPtr)> ("pre--", +[](const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return LiteralNumberPtr();
            }, MethodFlags::Pure | MethodFlags::Abstract),

            // Addition
            makeMethodBinding<LiteralNumberPtr (LiteralNumberPtr, LiteralNumberPtr)> ("+", +[](const LiteralNumberPtr &, const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return LiteralNumberPtr();
            }, MethodFlags::Pure | MethodFlags::Abstract),

            // Subtraction
            makeMethodBinding<LiteralNumberPtr (LiteralNumberPtr, LiteralNumberPtr)> ("-", +[](const LiteralNumberPtr &, const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return LiteralNumberPtr();
            }, MethodFlags::Pure | MethodFlags::Abstract),

            // Multiplication
            makeMethodBinding<LiteralNumberPtr (LiteralNumberPtr, LiteralNumberPtr)> ("*", +[](const LiteralNumberPtr &, const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return LiteralNumberPtr();
            }, MethodFlags::Pure | MethodFlags::Abstract),

            // Division
            makeMethodBinding<LiteralNumberPtr (LiteralNumberPtr, LiteralNumberPtr)> ("/", +[](const LiteralNumberPtr &, const LiteralNumberPtr &) {
                SysmelSelfSubclassResponsibility();
                return LiteralNumberPtr();
            }, MethodFlags::Pure | MethodFlags::Abstract),
        }},

        {"converting", {
            makeMethodBinding("asFloat", &LiteralNumber::asFloat, MethodFlags::Pure | MethodFlags::Abstract),
        }}
    };
}

TypePtr LiteralNumber::__asInferredTypeForWithModeInEnvironment__(const TypePtr &selfType, const ASTNodePtr &node, TypeInferenceMode mode, bool isMutable, bool concreteLiterals, const ASTAnalysisEnvironmentPtr &environment)
{
    if(concreteLiterals && node->isASTLiteralValueNode() && !environment->hasValidLiteralValueInferrenceType())
        return Float64::__staticType__();

    return SuperType::__asInferredTypeForWithModeInEnvironment__(selfType, node, mode, isMutable, concreteLiterals, environment);
}

bool LiteralNumber::isLiteralNumber() const
{
    return true;
}

double LiteralNumber::asFloat() const
{
    SysmelSelfSubclassResponsibility();
}

} // End of namespace Environment
} // End of namespace Sysmel