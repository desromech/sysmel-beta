#include "Environment/DeferredCompileTimeCodeFragment.hpp"
#include "Environment/Type.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/CompilationError.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"


namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<DeferredCompileTimeCodeFragment> localVariableTypeRegistration;

DeferredCompileTimeCodeFragmentPtr DeferredCompileTimeCodeFragment::make(ASTNodePtr codeFragment, ASTAnalysisEnvironmentPtr environment)
{
    auto result = basicMakeObject<DeferredCompileTimeCodeFragment> ();
    result->codeFragment = codeFragment;
    result->environment = environment;
    return result;
}

AnyValuePtr DeferredCompileTimeCodeFragment::analyzeAndEvaluateWithExpectedType(const TypePtr &expectedType)
{
    auto analyzer = basicMakeObject<ASTSemanticAnalyzer> ();
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
    return staticObjectCast<Type> (result);
}

AnyValuePtr DeferredCompileTimeCodeFragment::analyzeAndEvaluate()
{
    return analyzeAndEvaluateWithExpectedType(AnyValue::__staticType__());
}

void DeferredCompileTimeCodeFragment::analyzeAndEvaluateAsValuesForEnumType(const EnumTypePtr &enumType)
{
    auto analyzer = basicMakeObject<ASTSemanticAnalyzer> ();
    analyzer->environment = environment;
    analyzer->analyzeAndEvaluateAsValuesForEnumType(codeFragment, enumType);

    auto analysisError = analyzer->makeCompilationError();
    if(analysisError)
        analysisError->signal();
}

} // End of namespace Environment
} // End of namespace Sysmel