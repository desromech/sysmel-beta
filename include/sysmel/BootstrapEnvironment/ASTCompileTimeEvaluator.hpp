#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_COMPILE_TIME_EVALUATOR_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_COMPILE_TIME_EVALUATOR_HPP
#pragma once

#include "ASTVisitor.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a compile time evaluator of a previously analyzed AST.
 */
class ASTCompileTimeEvaluator : public SubtypeOf<ASTVisitor, ASTCompileTimeEvaluator>
{
public:
    static constexpr char const __typeName__[] = "ASTCompileTimeEvaluator";

    virtual AnyValuePtr visitCleanUpScopeNode(const ASTCleanUpScopeNodePtr &node);
    virtual AnyValuePtr visitClosureNode(const ASTClosureNodePtr &node);
    virtual AnyValuePtr visitIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &node);
    virtual AnyValuePtr visitCallNode(const ASTCallNodePtr &node);
    virtual AnyValuePtr visitLexicalScopeNode(const ASTLexicalScopeNodePtr &node);
    virtual AnyValuePtr visitLiteralValueNode(const ASTLiteralValueNodePtr &node);
    virtual AnyValuePtr visitMakeAssociationNode(const ASTMakeAssociationNodePtr &node);
    virtual AnyValuePtr visitMakeDictionaryNode(const ASTMakeDictionaryNodePtr &node);
    virtual AnyValuePtr visitMakeTupleNode(const ASTMakeTupleNodePtr &node);
    virtual AnyValuePtr visitMessageSendNode(const ASTMessageSendNodePtr &node);
    virtual AnyValuePtr visitQuasiQuoteNode(const ASTQuasiQuoteNodePtr &node);
    virtual AnyValuePtr visitQuoteNode(const ASTQuoteNodePtr &node);
    virtual AnyValuePtr visitSequenceNode(const ASTSequenceNodePtr &node);
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_COMPILE_TIME_EVALUATOR_HPP