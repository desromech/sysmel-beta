#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_DEFERRED_COMPILE_TIME_CODE_FRAGMENT_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_DEFERRED_COMPILE_TIME_CODE_FRAGMENT_HPP
#pragma once

#include "CompilerObject.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTAnalysisEnvironment);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(DeferredCompileTimeCodeFragment);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(EnumType);

/**
 * I am a compile time code fragment whose semantic analysis and evaluation is deferred into different phases of the compile time process.
 */
class DeferredCompileTimeCodeFragment : public SubtypeOf<CompilerObject, DeferredCompileTimeCodeFragment>
{
public:
    static constexpr char const __typeName__[] = "DeferredCompileTimeCodeFragment";

    static DeferredCompileTimeCodeFragmentPtr make(ASTNodePtr codeFragment, ASTAnalysisEnvironmentPtr environment);

    AnyValuePtr analyzeAndEvaluateWithExpectedType(const TypePtr &expectedType);
    TypePtr analyzeAndEvaluateAsTypeExpression();
    AnyValuePtr analyzeAndEvaluate();
    void analyzeAndEvaluateAsValuesForEnumType(const EnumTypePtr &enumType);

    ASTNodePtr codeFragment;
    ASTAnalysisEnvironmentPtr environment;
};
} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_DEFERRED_COMPILE_TIME_CODE_FRAGMENT_HPP
