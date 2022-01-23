#ifndef SYSMEL_ENVIRONMENT_AST_MAKE_ASSOCIATION_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_MAKE_ASSOCIATION_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am an AST node whose job is to make a tuple.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTMakeAssociationNode : public SubtypeOf<ASTNode, ASTMakeAssociationNode>
{
public:
    static constexpr char const __typeName__[] = "ASTMakeAssociationNode";

    virtual bool isASTMakeAssociationNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    virtual ASTNodePtr parseAsPatternMatchingCaseWith(const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    ASTNodePtr key;
    ASTNodePtr value;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_MAKE_ASSOCIATION_NODE_HPP