#ifndef SYSMEL_ENVIRONMENT_AST_TYPE_CONVERSION_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_TYPE_CONVERSION_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a node that is used for declaring or defining a type program entity.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTTypeConversionNode : public SubtypeOf<ASTNode, ASTTypeConversionNode>
{
public:
    static constexpr char const __typeName__[] = "ASTTypeConversionNode";

    virtual bool isASTTypeConversionNode() const override;

    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    ASTNodePtr expression;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_TYPE_CONVERSION_NODE_HPP