#include "Environment/SimpleType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/MetaType.hpp"


namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SimpleType> bootstrapTypeTypeRegistration;

bool SimpleType::isSimpleType() const
{
    return true;
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
    assert(newSupertype);
    auto newMeta = basicMakeObject<MetaType> ();
    metaType = newMeta;
    newMeta->setThisType(selfFromThis());

    setSupertype(newSupertype);
    newMeta->setSupertype(newSupertype->getType());

    addDefaultTypeConversionRules();
    newMeta->addDefaultTypeConversionRules();
}

} // End of namespace Environment
} // End of namespace Sysmel