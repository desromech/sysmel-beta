#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_COMPILE_TIME_CONSTANT_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_COMPILE_TIME_CONSTANT_HPP
#pragma once

#include "ModuleDefinedProgramEntity.hpp"
#include "TypeInferenceMode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(CompileTimeConstant)
SYSMEL_DECLARE_BOOTSTRAP_CLASS(DeferredCompileTimeCodeFragment)

/**
 * I am a compile time constant program entity.
 */
class CompileTimeConstant : public SubtypeOf<ModuleDefinedProgramEntity, CompileTimeConstant>
{
public:
    static constexpr char const __typeName__[] = "CompileTimeConstant";

    virtual bool isCompileTimeConstant() const override;

    virtual void ensureSemanticAnalysis() override;
    virtual ASTNodePtr analyzeIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;
    virtual AnyValuePtr getName() const override;


protected:
    friend class ASTSemanticAnalyzer;

    AnyValuePtr name;
    AnyValuePtr value;
    TypePtr valueType;

    DeferredCompileTimeCodeFragmentPtr valueCodeFragment;
    bool hasEvaluatedValueDefinition = false;
    bool isEvaluatingValueDefinition = false;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_COMPILE_TIME_CONSTANT_HPP