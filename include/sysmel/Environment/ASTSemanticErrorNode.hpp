#ifndef SYSMEL_ENVIRONMENT_AST_SEMANTIC_ERROR_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_SEMANTIC_ERROR_NODE_HPP
#pragma once

#include "ASTErrorNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTSemanticErrorNode : public SubtypeOf<ASTErrorNode, ASTSemanticErrorNode>
{
public:
    static constexpr char const __typeName__[] = "ASTSemanticErrorNode";

    virtual bool isASTSemanticErrorNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual CompilationErrorPtr asCompilationError() override;

    std::string errorMessage;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_SEMANTIC_ERROR_NODE_HPP