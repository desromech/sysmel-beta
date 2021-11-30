#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SSA_COMPILER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SSA_COMPILER_HPP
#pragma once

#include "ASTVisitor.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(SpecificMethod)
SYSMEL_DECLARE_BOOTSTRAP_CLASS(CompiledMethod)
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAFunction)
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSACodeRegion)
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSABuilder)

/**
 * I am a compile time evaluator of a previously analyzed AST.
 */
class ASTSSACompiler : public SubtypeOf<ASTVisitor, ASTSSACompiler>
{
public:
    static constexpr char const __typeName__[] = "ASTSSACompiler";

    SSAValuePtr visitNodeForValue(const ASTNodePtr &node);

    void compileMethodBody(const CompiledMethodPtr &method, const SSAFunctionPtr &ssaFunction, const ASTNodePtr &node);

    virtual AnyValuePtr visitCallNode(const ASTCallNodePtr &node) override;
    virtual AnyValuePtr visitLiteralValueNode(const ASTLiteralValueNodePtr &node) override;
    virtual AnyValuePtr visitMessageSendNode(const ASTMessageSendNodePtr &node) override;
    virtual AnyValuePtr visitSequenceNode(const ASTSequenceNodePtr &node) override;
    
    virtual AnyValuePtr visitVariableAccessNode(const ASTVariableAccessNodePtr &node);

    virtual AnyValuePtr visitValueAsVoidTypeConversionNode(const ASTValueAsVoidTypeConversionNodePtr &node) override;

    SpecificMethodPtr currentMethod;
    SSAFunctionPtr currentSSAFunction;
    SSACodeRegionPtr currentCodeRegion;
    SSABuilderPtr builder;

    std::unordered_map<AnyValuePtr, SSAValuePtr> localVariableMap;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SSA_COMPILER_HPP