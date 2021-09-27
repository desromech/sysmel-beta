#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_LANGUAGE_SUPPORT_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_LANGUAGE_SUPPORT_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTAnalysisEnvironment);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(CompiledMethod);

/**
 * I am the interface for all of the language adapters. A language adapters takes care of actually
 * parsing a source language into the language independent AST, and to also define the base
 * environment for performing semantic analysis on them.
 */
class LanguageSupport : public SubtypeOf<CompilerObject, LanguageSupport>
{
public:
    static constexpr char const __typeName__[] = "LanguageSupport";

    static MethodCategories __instanceMethods__();

    virtual ASTAnalysisEnvironmentPtr createDefaultAnalysisEnvironment() const;
    
    virtual ASTNodePtr parseSourceStringNamed(const std::string &sourceString, const std::string &sourceStringName) const;
    virtual ASTNodePtr parseFileNamed(const std::string &fileName) const;

    virtual CompiledMethodPtr analyzeASTInEnvironment(const ASTNodePtr &ast, const ASTAnalysisEnvironmentPtr &environment) const;

    virtual AnyValuePtr evaluateSourceStringNamed(const std::string &sourceString, const std::string &sourceStringName) const;
    virtual AnyValuePtr evaluateFileNamed(const std::string &fileName) const;

};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_LANGUAGE_SUPPORT_HPP