#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_NAMESPACE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_NAMESPACE_HPP
#pragma once

#include "ModuleDefinedProgramEntity.hpp"
#include <unordered_map>

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(Namespace);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSANamespace);

/**
 * I am a namespace.
 */
class Namespace : public SubtypeOf<ModuleDefinedProgramEntity, Namespace>
{
public:
    static constexpr char const __typeName__[] = "Namespace";

    static constexpr bool __isDynamicCompileTimeType__ = false;
    static constexpr bool __isLiteralValueMessageAnalyzer__ = true;

    static NamespacePtr makeWithName(const AnyValuePtr &name);

    virtual bool isNamespace() const override;

    virtual AnyValuePtr getName() const override;

    virtual ASTNodePtr analyzeMessageSendNode(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    virtual void recordChildProgramEntityDefinition(const ProgramEntityPtr &newChild) override;
    virtual void bindSymbolWithVisibility(const AnyValuePtr &symbol, ProgramEntityVisibility visibility, const ProgramEntityPtr &binding) override;
    virtual AnyValuePtr lookupExportedSymbolFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope) override;
    virtual AnyValuePtr lookupLocalSymbolFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope) override;
    
    virtual AnyValuePtr lookupLocalMacroSelector(const AnyValuePtr &selector) override;
    virtual AnyValuePtr lookupLocalSelector(const AnyValuePtr &selector) override;
    virtual AnyValuePtr lookupLocalMacroFallbackSelector(const AnyValuePtr &selector) override;

    virtual void addMacroMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector) override;
    virtual void addMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector) override;
    virtual void addMacroFallbackMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector) override;

    virtual SExpression asSExpression() const override;
    virtual SExpression asFullDefinitionSExpression() const override;

    virtual SSAValuePtr asSSAValueRequiredInPosition(const ASTSourcePositionPtr &requiredSourcePosition) override;
    
protected:
    AnyValuePtr name;

    ProgramEntityPtrList children;
    std::unordered_map<AnyValuePtr, ProgramEntityVisibilityWithBinding> bindings;

    MethodDictionaryPtr macroMethodDictionary;
    MethodDictionaryPtr methodDictionary;
    MethodDictionaryPtr macroFallbackMethodDictionary;

    SSANamespacePtr ssaNamespace;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_NAMESPACE_HPP