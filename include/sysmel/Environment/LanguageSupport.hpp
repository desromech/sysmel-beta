#ifndef SYSMEL_ENVIRONMENT_AST_LANGUAGE_SUPPORT_HPP
#define SYSMEL_ENVIRONMENT_AST_LANGUAGE_SUPPORT_HPP
#pragma once

#include "CompilerObject.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTAnalysisEnvironment);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(LexicalScope);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(CompiledMethod);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(LanguageSupport);

/**
 * I am the interface for all of the language adapters. A language adapters takes care of actually
 * parsing a source language into the language independent AST, and to also define the base
 * environment for performing semantic analysis on them.
 */
class SYSMEL_COMPILER_LIB_EXPORT LanguageSupport : public SubtypeOf<CompilerObject, LanguageSupport>
{
public:
    static constexpr char const __typeName__[] = "LanguageSupport";

    static MethodCategories __instanceMethods__();

    virtual ASTAnalysisEnvironmentPtr createDefaultAnalysisEnvironment();
    virtual LexicalScopePtr createDefaultTopLevelLexicalScope();

    virtual ASTAnalysisEnvironmentPtr createMakeLiteralArrayAnalysisEnvironment();
    virtual LexicalScopePtr createMakeLiteralArrayTopLevelLexicalScope();

    virtual ASTNodePtr parseSourceStringNamed(const std::string &sourceString, const std::string &sourceStringName);
    virtual ASTNodePtr parseFileNamed(const std::string &fileName);

    virtual CompiledMethodPtr analyzeASTInEnvironment(const ASTNodePtr &ast, const ASTAnalysisEnvironmentPtr &environment);

    virtual CompiledMethodPtr semanticAnalyzeStringNamed(const std::string &sourceString, const std::string &sourceStringName);
    virtual CompiledMethodPtr semanticAnalyzeFileNamed(const std::string &fileName);

    virtual AnyValuePtr evaluateSourceStringNamed(const std::string &sourceString, const std::string &sourceStringName);
    virtual AnyValuePtr evaluateFileNamed(const std::string &fileName);

};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_LANGUAGE_SUPPORT_HPP