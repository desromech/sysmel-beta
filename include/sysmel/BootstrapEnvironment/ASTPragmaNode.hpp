#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_PRAGMA_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_PRAGMA_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTPragmaNode : public SubtypeOf<ASTNode, ASTPragmaNode>
{
public:
    static constexpr char const __typeName__[] = "ASTPragmaNode";

    virtual bool isASTPragmaNode() const override;
    virtual SExpression asSExpression() const override;

    ASTNodePtr selector;
    ASTNodePtrList arguments;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_PRAGMA_NODE_HPP