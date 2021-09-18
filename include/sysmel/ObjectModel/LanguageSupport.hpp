#ifndef SYSMEL_COMPILER_OBJECT_MODEL_AST_LANGUAGE_SUPPORT_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_AST_LANGUAGE_SUPPORT_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

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
    
    virtual ASTNodePtr parseSourceStringNamed(const std::string &sourceString, const std::string &sourceStringName) const;
    virtual ASTNodePtr parseFileNamed(const std::string &fileName) const;

    virtual AnyValuePtr evaluateSourceStringNamed(const std::string &sourceString, const std::string &sourceStringName) const;
    virtual AnyValuePtr evaluateFileNamed(const std::string &fileName) const;

};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_AST_LANGUAGE_SUPPORT_HPP