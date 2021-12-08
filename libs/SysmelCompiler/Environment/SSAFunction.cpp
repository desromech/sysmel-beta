#include "Environment/SSAFunction.hpp"
#include "Environment/SSACodeRegion.hpp"
#include "Environment/SSACodeRegionLocalValue.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/FunctionalType.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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
        return asFullDefinitionSExpression();
    
    return SExpressionList{{SExpressionIdentifier{{"function"}},
        name ? name->asSExpression() : nullptr,
        intrinsicName ? intrinsicName->asSExpression() : nullptr,
    }};
}

SExpression SSAFunction::asFullDefinitionSExpression() const
{
    enumerateLocalValues();
    return SExpressionList{{SExpressionIdentifier{{"function"}},
        name ? name->asSExpression() : nullptr,
        intrinsicName ? intrinsicName->asSExpression() : nullptr,
        functionalType ? functionalType->asSExpression() : nullptr,
        sourcePosition ? sourcePosition->asSExpression() : nullptr,
        mainCodeRegion ? mainCodeRegion->asFullDefinitionSExpression() : nullptr,
    }};
}

void SSAFunction::enumerateLocalValues() const
{
    SSACodeRegionLocalValueEnumerationState state;
    mainCodeRegion->enumerateLocalValues(state);
}
} // End of namespace Environment
} // End of namespace Sysmel
