#include "sysmel/BootstrapEnvironment/SSAFunction.hpp"
#include "sysmel/BootstrapEnvironment/SSACodeRegion.hpp"
#include "sysmel/BootstrapEnvironment/SSACodeRegionLocalValue.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/FunctionalType.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAFunction> SSAFunctionTypeRegistration;

bool SSAFunction::isSSAFunction() const
{
    return true;
}

AnyValuePtr SSAFunction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitFunction(selfFromThis());
}

AnyValuePtr SSAFunction::getName() const
{
    return name;
}

TypePtr SSAFunction::getValueType() const
{
    return functionalType;
}

const FunctionalTypePtr &SSAFunction::getFunctionalType()
{
    return functionalType;
}

void SSAFunction::initializeWithNameAndType(const AnyValuePtr &newName, const FunctionalTypePtr &newFunctionalType)
{
    name = newName;
    functionalType = newFunctionalType;

    mainCodeRegion = basicMakeObject<SSACodeRegion> ();
    mainCodeRegion->setFunctionalType(newFunctionalType);
}

void SSAFunction::setDeclarationPosition(const ASTSourcePositionPtr &position)
{
    sourcePosition = position;
    mainCodeRegion->setSourcePosition(position);
}

void SSAFunction::setDefinitionPosition(const ASTSourcePositionPtr &position)
{
    mainCodeRegion->setSourcePosition(position);
}

SSACodeRegionPtr SSAFunction::getMainCodeRegion() const
{
    return mainCodeRegion;
}

const AnyValuePtr &SSAFunction::getIntrinsicName() const
{
    return intrinsicName;
}

void SSAFunction::setIntrinsicName(const AnyValuePtr &newIntrinsicName)
{
    intrinsicName = newIntrinsicName;
}

const ProgramEntityPtr &SSAFunction::getSourceProgramEntity()
{
    return sourceProgramEntity;
}

void SSAFunction::setSourceProgramEntity(const ProgramEntityPtr &entity)
{
    sourceProgramEntity = entity;
}

SExpression SSAFunction::asSExpression() const
{
    if(functionalType->isClosureType() && validAnyValue(name)->isAnonymousNameSymbol())
        return asFullSExpression();
    
    return SExpressionList{{SExpressionIdentifier{{"function"}},
        name ? name->asSExpression() : nullptr,
        intrinsicName ? intrinsicName->asSExpression() : nullptr,
    }};
}

SExpression SSAFunction::asFullSExpression() const
{
    enumerateLocalValues();
    return SExpressionList{{SExpressionIdentifier{{"function"}},
        name ? name->asSExpression() : nullptr,
        intrinsicName ? intrinsicName->asSExpression() : nullptr,
        functionalType ? functionalType->asSExpression() : nullptr,
        sourcePosition ? sourcePosition->asSExpression() : nullptr,
        mainCodeRegion ? mainCodeRegion->asFullSExpression() : nullptr,
    }};
}

void SSAFunction::enumerateLocalValues() const
{
    SSACodeRegionLocalValueEnumerationState state;
    mainCodeRegion->enumerateLocalValues(state);
}
} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius
