#include "Environment/SSACodeRegionCapture.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSACodeRegionCapture> SSACodeRegionCaptureTypeRegistration;

SSACodeRegionCapturePtr SSACodeRegionCapture::make(const TypePtr &valueType)
{
    auto result = basicMakeObject<SSACodeRegionCapture> ();
    result->valueType = valueType;
    return result;
}

bool SSACodeRegionCapture::isSSACodeRegionCapture() const
{
    return true;
}

AnyValuePtr SSACodeRegionCapture::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitCodeRegionCapture(selfFromThis());
}

SExpression SSACodeRegionCapture::asFullDefinitionSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"capture"}},
        LargeInteger{localValueIndex},
        valueType->asSExpression(),
        declarationSourcePosition ? declarationSourcePosition->asSExpression() : nullptr,
        definitionSourcePosition ? definitionSourcePosition->asSExpression() : nullptr,
    }};
}

void SSACodeRegionCapture::setDeclarationPosition(const ASTSourcePositionPtr &position)
{
    declarationSourcePosition = position;
}

void SSACodeRegionCapture::setDefinitionPosition(const ASTSourcePositionPtr &position)
{
    definitionSourcePosition = position;
}

TypePtr SSACodeRegionCapture::getValueType() const
{
    return valueType;
}

} // End of namespace Environment
} // End of namespace Sysmel