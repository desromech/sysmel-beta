#ifndef SYSMEL_ENVIRONMENT_AST_PRAGMA_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_PRAGMA_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTPragmaNode : public SubtypeOf<ASTNode, ASTPragmaNode>
{
public:
    static constexpr char const __typeName__[] = "ASTPragmaNode";

    virtual bool isASTPragmaNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
    
    virtual void childrenDo(const ASTIterationBlock &aBlock) override;
    virtual ASTPragmaNodePtr getPragmaNamed(const AnyValuePtr &requestedPragmaSelector) override;

    ASTNodePtr selector;
    ASTNodePtrList arguments;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_PRAGMA_NODE_HPP