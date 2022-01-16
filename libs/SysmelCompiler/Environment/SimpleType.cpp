#include "Environment/SimpleType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/MetaType.hpp"
#include "Environment/TypeVisitor.hpp"


namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SimpleType> bootstrapTypeTypeRegistration;

bool SimpleType::isSimpleType() const
{
    return true;
}

AnyValuePtr SimpleType::acceptTypeVisitor(const TypeVisitorPtr &visitor)
{
    return visitor->visitSimpleType(selfFromThis());
}

TypePtr SimpleType::getType() const
{
    return metaType;
}

void SimpleType::setType(const TypePtr &theMetaType)
{
    metaType = theMetaType;
}

void SimpleType::setSupertypeAndImplicitMetaType(const TypePtr &newSupertype)
{
    sysmelAssert(newSupertype);
    auto newMeta = basicMakeObject<MetaType> ();
    metaType = newMeta;
    newMeta->setThisType(selfFromThis());
    recordChildProgramEntityDefinition(metaType);

    setSupertype(newSupertype);
    newMeta->setSupertype(newSupertype->getType());

    addDefaultTypeConversionRules();
    newMeta->addDefaultTypeConversionRules();
}

} // End of namespace Environment
} // End of namespace Sysmel