#ifndef SYSMEL_COMPILER_OBJECT_MODEL_AST_CLEANUP_SCOPE_NODE_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_AST_CLEANUP_SCOPE_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTCleanUpScopeNode : public SubtypeOf<ASTNode, ASTCleanUpScopeNode>
{
public:
    static constexpr char const __typeName__[] = "ASTCleanUpScopeNode";

    virtual bool isASTCleanUpScopeNode() const override;

    AnyValuePtr body;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_AST_CLEANUP_SCOPE_NODE_HPP