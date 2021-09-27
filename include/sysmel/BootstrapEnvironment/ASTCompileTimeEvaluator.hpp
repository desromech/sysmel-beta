#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_COMPILE_TIME_EVALUATOR_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_COMPILE_TIME_EVALUATOR_HPP
#pragma once

#include "ASTVisitor.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a compile time evaluator of a previously analyzed AST.
 */
class ASTCompileTimeEvaluator : public SubtypeOf<ASTVisitor, ASTCompileTimeEvaluator>
{
public:
    static constexpr char const __typeName__[] = "ASTCompileTimeEvaluator";

};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_COMPILE_TIME_EVALUATOR_HPP