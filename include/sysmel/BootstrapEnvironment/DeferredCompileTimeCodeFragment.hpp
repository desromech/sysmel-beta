#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_DEFERRED_COMPILE_TIME_CODE_FRAGMENT_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_DEFERRED_COMPILE_TIME_CODE_FRAGMENT_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTAnalysisEnvironment);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(DeferredCompileTimeCodeFragment);

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

    ASTNodePtr codeFragment;
    ASTAnalysisEnvironmentPtr environment;
};
} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_DEFERRED_COMPILE_TIME_CODE_FRAGMENT_HPP
