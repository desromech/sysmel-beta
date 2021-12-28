#ifndef SYSMEL_ENVIRONMENT_AST_MAKE_DICTIONARY_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_MAKE_DICTIONARY_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am an AST node whose job is to make a tuple.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTMakeDictionaryNode : public SubtypeOf<ASTNode, ASTMakeDictionaryNode>
{
public:
    static constexpr char const __typeName__[] = "ASTMakeDictionaryNode";

    virtual bool isASTMakeDictionaryNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual void childrenDo(const ASTIterationBlock &aBlock) override;

    ASTNodePtrList elements;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_MAKE_DICTIONARY_NODE_HPP