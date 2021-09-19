#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_NODE_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

class ASTSourcePosition;
typedef std::shared_ptr<ASTSourcePosition> ASTSourcePositionPtr;

class ASTArgumentDefinitionNode;
typedef std::shared_ptr<ASTArgumentDefinitionNode> ASTArgumentDefinitionNodePtr;
typedef std::vector<ASTArgumentDefinitionNodePtr> ASTArgumentDefinitionNodePtrList;

class ASTMessageChainMessageNode;
typedef std::shared_ptr<ASTMessageChainMessageNode> ASTMessageChainMessageNodePtr;
typedef std::vector<ASTMessageChainMessageNodePtr> ASTMessageChainMessageNodePtrList;

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTNode : public SubtypeOf<CompilerObject, ASTNode>
{
public:
    static constexpr char const __typeName__[] = "ASTNode";

    ASTNode();

    virtual bool isASTNode() const override;

    virtual std::string printString() const;

    ASTSourcePositionPtr sourcePosition;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_NODE_HPP