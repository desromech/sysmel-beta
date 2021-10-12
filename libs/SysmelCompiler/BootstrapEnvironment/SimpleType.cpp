#include "sysmel/BootstrapEnvironment/SimpleType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"


namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static SimpleTypeRegistration<SimpleType> bootstrapTypeTypeRegistration;

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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius