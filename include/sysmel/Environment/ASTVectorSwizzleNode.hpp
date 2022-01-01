#ifndef SYSMEL_ENVIRONMENT_AST_VECTOR_SWIZZLE_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_VECTOR_SWIZZLE_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am an AST node whose job is to make a tuple.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTVectorSwizzleNode : public SubtypeOf<ASTNode, ASTVectorSwizzleNode>
{
public:
    static constexpr char const __typeName__[] = "ASTVectorSwizzleNode";

    virtual bool isASTVectorSwizzleNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    ASTNodePtr vector;
    std::vector<uint32_t> selectedElements;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_VECTOR_SWIZZLE_NODE_HPP