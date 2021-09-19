#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_MAKE_DICTIONARY_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_MAKE_DICTIONARY_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am an AST node whose job is to make a tuple.
 */
class ASTMakeDictionaryNode : public SubtypeOf<ASTNode, ASTMakeDictionaryNode>
{
public:
    static constexpr char const __typeName__[] = "ASTMakeDictionaryNode";

    virtual bool isASTMakeDictionaryNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    ASTNodePtrList elements;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_MAKE_DICTIONARY_NODE_HPP