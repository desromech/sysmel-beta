#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SEMANTIC_ANALYZER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SEMANTIC_ANALYZER_HPP
#pragma once

#include "ASTVisitor.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTAnalysisEnvironment)

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTSemanticAnalyzer : public SubtypeOf<ASTVisitor, ASTSemanticAnalyzer>
{
public:
    static constexpr char const __typeName__[] = "ASTSemanticAnalyzer";

    ASTAnalysisEnvironmentPtr environment;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SEMANTIC_ANALYZER_HPP