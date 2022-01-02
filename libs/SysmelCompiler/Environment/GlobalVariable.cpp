#include "Environment/GlobalVariable.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/DeferredCompileTimeCodeFragment.hpp"
#include "Environment/PointerLikeType.hpp"
#include "Environment/Type.hpp"
#include "Environment/SSAGlobalVariable.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"


namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<GlobalVariable> localVariableTypeRegistration;

void GlobalVariable::ensureSemanticAnalysis()
{
    SuperType::ensureSemanticAnalysis();
    ensureEvaluationOfInitialValue();
    asSSAValueRequiredInPosition(ASTSourcePosition::empty());
}

void GlobalVariable::ensureEvaluationOfInitialValue()
{
    if(hasEvaluatedInitialValueCodeFragment)
        return;

    auto evaluatedValue = initialValueCodeFragment ? initialValueCodeFragment->analyzeAndEvaluate() : getValueType()->defaultValue();
    if(isMutable_)
    {
        sysmelAssert(referenceType->isReferenceLikeType());
        currentValueOrValueBox = validAnyValue(evaluatedValue)->asMutableStoreValue();
        referenceValue = staticObjectCast<PointerLikeType> (referenceType)->makeWithValue(currentValueOrValueBox);
    }
    else
    {
        referenceValue = currentValueOrValueBox = evaluatedValue;
    }

    hasEvaluatedInitialValueCodeFragment = true;
    if(ssaGlobalVariable)
        ssaGlobalVariable->setInitialValue(currentValueOrValueBox);
}

SSAValuePtr GlobalVariable::asSSAValueRequiredInPosition(const ASTSourcePositionPtr &)
{
    if(ssaGlobalVariable)
        return ssaGlobalVariable;

    ssaGlobalVariable = basicMakeObject<SSAGlobalVariable> ();
    ssaGlobalVariable->setName(name);
    ssaGlobalVariable->setContentAndValueType(valueType, referenceType);
    ssaGlobalVariable->setVisibility(visibility);
    ssaGlobalVariable->setDllLinkageMode(dllLinkageMode);
    ssaGlobalVariable->setExternalLanguageMode(externalLanguageMode);
    ssaGlobalVariable->setInitialValue(currentValueOrValueBox);
    // TODO: Support externally defined global variables.

    // Add the global onto its ssa parent.
    auto parentEntity = getParentProgramEntity();
    if(parentEntity)
    {
        auto parentSSAValue = parentEntity->asProgramEntitySSAValue();
        sysmelAssert(parentSSAValue->isSSAProgramEntity());
        parentSSAValue.staticAs<SSAProgramEntity> ()->addChild(ssaGlobalVariable);
    }

    return ssaGlobalVariable;
}

SExpression GlobalVariable::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{"localVariable"},
        validAnyValue(name)->asSExpression(),
    }};
}

SExpression GlobalVariable::asFullDefinitionSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{"localVariable"},
        validAnyValue(name)->asSExpression(),
        isMutable_,
        valueType->asSExpression(),
        referenceType->asSExpression(),
    }};
}

bool GlobalVariable::isGlobalVariable() const
{
    return true;
}

AnyValuePtr GlobalVariable::findStoreBindingInCompileTime(const CompileTimeCleanUpScopePtr &compileTimeCleanUpScope)
{
    (void)compileTimeCleanUpScope;
    ensureEvaluationOfInitialValue();
    return referenceValue;
}

} // End of namespace Environment
} // End of namespace Sysmel