#include "sysmel/BootstrapEnvironment/SimpleType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/MetaType.hpp"


namespace SysmelMoebius
{
namespace BootstrapEnvironment
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
    auto newMeta = std::make_shared<MetaType> ();
    metaType = newMeta;
    newMeta->setThisType(shared_from_this());

    setSupertype(newSupertype);
    newMeta->setSupertype(newSupertype->getType());

    addDefaultTypeConversionRules();
    newMeta->addDefaultTypeConversionRules();
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius