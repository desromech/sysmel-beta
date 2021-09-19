#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SOURCE_CODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SOURCE_CODE_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTSourceCode : public SubtypeOf<CompilerObject, ASTSourceCode>
{
public:
    static constexpr char const __typeName__[] = "ASTSourceCode";

    virtual bool isASTSourceCode() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SOURCE_CODE_HPP