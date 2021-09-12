#include "sysmel/ObjectModel/MetaType.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace ObjectModel
{
static BootstrapTypeRegistration<MetaType> metaTypeTypeRegistration;

void MetaType::setThisType(const TypePtr &instanceType)
{
    thisType = instanceType;
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

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius