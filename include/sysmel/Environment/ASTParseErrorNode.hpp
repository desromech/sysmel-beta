#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_PARSE_ERROR_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_PARSE_ERROR_NODE_HPP
#pragma once

#include "ASTErrorNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTParseErrorNode : public SubtypeOf<ASTErrorNode, ASTParseErrorNode>
{
public:
    static constexpr char const __typeName__[] = "ASTParseErrorNode";

    virtual bool isASTParseErrorNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual CompilationErrorPtr asCompilationError() override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_PARSE_ERROR_NODE_HPP