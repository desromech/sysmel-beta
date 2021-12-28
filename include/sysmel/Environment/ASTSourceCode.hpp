#ifndef SYSMEL_ENVIRONMENT_AST_SOURCE_CODE_HPP
#define SYSMEL_ENVIRONMENT_AST_SOURCE_CODE_HPP
#pragma once

#include "CompilerObject.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTSourceCode);

/**
 * I am the interface for all of the language independent AST nodes.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTSourceCode : public SubtypeOf<CompilerObject, ASTSourceCode>
{
public:
    static constexpr char const __typeName__[] = "ASTSourceCode";

    virtual bool isASTSourceCode() const override;
    virtual SExpression asSExpression() const override;

    std::string name;
    std::string contents;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_SOURCE_CODE_HPP