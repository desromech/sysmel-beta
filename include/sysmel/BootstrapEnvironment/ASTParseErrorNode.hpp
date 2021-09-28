#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_PARSE_ERROR_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_PARSE_ERROR_NODE_HPP
#pragma once

#include "ASTNode.hpp"
#include "ParseError.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTParseErrorNode : public SubtypeOf<ASTNode, ASTParseErrorNode>
{
public:
    static constexpr char const __typeName__[] = "ASTParseErrorNode";

    virtual bool isASTParseErrorNode() const override;
    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    ParseErrorPtr asParseError();

    std::string errorMessage;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_PARSE_ERROR_NODE_HPP