#ifndef SYSMEL_ENVIRONMENT_AST_ERROR_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_ERROR_NODE_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(CompilationError);

/**
 * I am the interface for all of the language independent AST nodes.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTErrorNode : public SubtypeOf<ASTNode, ASTErrorNode>
{
public:
    static constexpr char const __typeName__[] = "ASTASTErrorNode";

    virtual bool isASTErrorNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual CompilationErrorPtr asCompilationError();

    virtual ASTNodePtr parseAsPatternNode() override;

    std::string errorMessage;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_SEMANTIC_ERROR_NODE_HPP