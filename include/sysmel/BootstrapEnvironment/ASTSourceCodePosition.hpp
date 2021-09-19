#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SOURCE_CODE_POSITION_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SOURCE_CODE_POSITION_HPP
#pragma once

#include "ASTSourceCode.hpp"
#include "ASTSourcePosition.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTSourceCodePosition : public SubtypeOf<ASTSourcePosition, ASTSourceCodePosition>
{
public:
    static constexpr char const __typeName__[] = "ASTSourceCodePosition";

    virtual bool isASTSourceCodePosition() const override;
    virtual SExpression asSExpression() const override;

    ASTSourceCodePtr sourceCode;
    size_t startPosition = 0;
    size_t startLine = 0;
    size_t startColumn = 0;

    size_t endPosition = 0;
    size_t endLine = 0;
    size_t endColumn = 0;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SOURCE_CODE_POSITION_HPP