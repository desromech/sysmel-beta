#ifndef SYSMEL_ENVIRONMENT_AST_TRAP_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_TRAP_NODE_HPP
#pragma once

#include "ASTNode.hpp"
#include "TrapReason.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am used for generating a runtime error. I might be compiled to throwing an exception, or aborting the program execution.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTTrapNode : public SubtypeOf<ASTNode, ASTTrapNode>
{
public:
    static constexpr char const __typeName__[] = "ASTTrapNode";

    virtual bool isASTTrapNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    TrapReason reason = TrapReason::Generic;
    std::string reasonMessage;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_TRAP_NODE_HPP
