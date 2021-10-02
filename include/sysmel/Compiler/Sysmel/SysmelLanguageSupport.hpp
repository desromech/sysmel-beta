#ifndef SYSMEL_COMPILER_SYSMEL_LANGUAGE_SUPPORT_HPP
#define SYSMEL_COMPILER_SYSMEL_LANGUAGE_SUPPORT_HPP
#pragma once

#include "sysmel/BootstrapEnvironment/LanguageSupport.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

class SysmelLanguageSupport;
typedef std::shared_ptr<SysmelLanguageSupport> SysmelLanguageSupportPtr;

/**
 * I am the interface for all of the language adapters. A language adapters takes care of actually
 * parsing a source language into the language independent AST, and to also define the base
 * environment for performing semantic analysis on them.
 */
class SysmelLanguageSupport : public SubtypeOf<LanguageSupport, SysmelLanguageSupport>
{
public:
    static constexpr char const __typeName__[] = "SysmelLanguageSupport";

    static MethodCategories __typeMethods__();

    static SysmelLanguageSupportPtr uniqueInstance();

    virtual LexicalScopePtr createDefaultTopLevelLexicalScope() override;
    virtual LexicalScopePtr createMakeLiteralArrayTopLevelLexicalScope() override;

    virtual ASTNodePtr parseSourceStringNamed(const std::string &sourceString, const std::string &sourceStringName) override;

protected:
    LexicalScopePtr keywordScope;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_SYSMEL_LANGUAGE_SUPPORT_HPP
