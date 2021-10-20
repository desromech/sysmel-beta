#include "sysmel/BootstrapEnvironment/DeferredCompileTimeCodeFragment.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticAnalyzer.hpp"
#include "sysmel/BootstrapEnvironment/CompilationError.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"


namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<DeferredCompileTimeCodeFragment> localVariableTypeRegistration;

DeferredCompileTimeCodeFragmentPtr DeferredCompileTimeCodeFragment::make(ASTNodePtr codeFragment, ASTAnalysisEnvironmentPtr environment)
{
    auto result = std::make_shared<DeferredCompileTimeCodeFragment> ();
    result->codeFragment = codeFragment;
    result->environment = environment;
    return result;
}

AnyValuePtr DeferredCompileTimeCodeFragment::analyzeAndEvaluateWithExpectedType(const TypePtr &expectedType)
{
    auto analyzer = std::make_shared<ASTSemanticAnalyzer> ();
    analyzer->environment = environment;
    codeFragment = analyzer->analyzeNodeIfNeededWithExpectedType(codeFragment, expectedType);
    auto analysisError = analyzer->makeCompilationError();
    if(analysisError)
        analysisError->signal();

    return analyzer->evaluateInCompileTime(codeFragment);
}

TypePtr DeferredCompileTimeCodeFragment::analyzeAndEvaluateAsTypeExpression()
{
    auto result = analyzeAndEvaluateWithExpectedType(Type::__staticType__());
    assert(result->isType());
    return std::static_pointer_cast<Type> (result);
}

AnyValuePtr DeferredCompileTimeCodeFragment::analyzeAndEvaluate()
{
    return analyzeAndEvaluateWithExpectedType(AnyValue::__staticType__());
}



} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius