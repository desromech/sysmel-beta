#ifndef SYSMEL_ENVIRONMENT_AST_METHOD_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_METHOD_NODE_HPP
#pragma once

#include "ASTFunctionalNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a node that is used for defining methods.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTMethodNode : public SubtypeOf<ASTFunctionalNode, ASTMethodNode>
{
public:
    static constexpr char const __typeName__[] = "ASTMethodNode";

    virtual bool isASTMethodNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_METHOD_NODE_HPP