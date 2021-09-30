#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_LANGUAGE_SUPPORT_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_LANGUAGE_SUPPORT_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTAnalysisEnvironment);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(IdentifierLookupScope);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(CompiledMethod);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(LanguageSupport);

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

    virtual ASTAnalysisEnvironmentPtr createDefaultAnalysisEnvironment();
    virtual IdentifierLookupScopePtr createDefaultTopLevelIdentifierLookupScope();

    virtual ASTAnalysisEnvironmentPtr createMakeLiteralArrayAnalysisEnvironment();
    virtual IdentifierLookupScopePtr createMakeLiteralArrayTopLevelIdentifierLookupScope();

    
    virtual ASTNodePtr parseSourceStringNamed(const std::string &sourceString, const std::string &sourceStringName);
    virtual ASTNodePtr parseFileNamed(const std::string &fileName);

    virtual CompiledMethodPtr analyzeASTInEnvironment(const ASTNodePtr &ast, const ASTAnalysisEnvironmentPtr &environment);

    virtual CompiledMethodPtr semanticAnalyzeStringNamed(const std::string &sourceString, const std::string &sourceStringName);
    virtual CompiledMethodPtr semanticAnalyzeFileNamed(const std::string &fileName);

    virtual AnyValuePtr evaluateSourceStringNamed(const std::string &sourceString, const std::string &sourceStringName);
    virtual AnyValuePtr evaluateFileNamed(const std::string &fileName);

};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_LANGUAGE_SUPPORT_HPP