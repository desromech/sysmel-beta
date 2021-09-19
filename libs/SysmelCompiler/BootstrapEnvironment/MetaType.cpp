#include "sysmel/BootstrapEnvironment/MetaType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<MetaType> metaTypeTypeRegistration;

void MetaType::setThisType(const TypePtr &instanceType)
{
    thisType = instanceType;
}

bool MetaType::isMetaType() const
{
    return true;
}

TypePtr MetaType::getInstanceType()
{
    return thisType;
}

TypePtr MetaType::getMetaType()
{
    return shared_from_this();
}

std::string MetaType::printString() const
{
    return thisType->printString() + " __type__";
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius