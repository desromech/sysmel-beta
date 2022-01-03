#ifndef SYSMEL_ENVIRONMENT_AST_MAKE_VARIANT_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_MAKE_VARIANT_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am an AST node whose job is to make a tuple.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTMakeVariantNode : public SubtypeOf<ASTNode, ASTMakeVariantNode>
{
public:
    static constexpr char const __typeName__[] = "ASTMakeVariantNode";

    virtual bool isASTMakeVariantNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    ASTNodePtr variantType;
    ASTNodePtr value;
    uint64_t typeSelectorIndex = 0;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_MAKE_VARIANT_NODE_HPP