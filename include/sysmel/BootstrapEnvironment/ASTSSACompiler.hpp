#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SSA_COMPILER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SSA_COMPILER_HPP
#pragma once

#include "ASTVisitor.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a compile time evaluator of a previously analyzed AST.
 */
class ASTSSACompiler : public SubtypeOf<ASTVisitor, ASTSSACompiler>
{
public:
    static constexpr char const __typeName__[] = "ASTSSACompiler";
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SSA_COMPILER_HPP