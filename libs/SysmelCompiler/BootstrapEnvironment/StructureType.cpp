#include "sysmel/BootstrapEnvironment/StructureType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<StructureTypeValue> StructureTypeValueTypeRegistration;

bool StructureType::isStructureType() const
{
    return true;
}

AnyValuePtr StructureType::basicNewValue()
{
    auto result = std::make_shared<StructureTypeValue> ();
    result->type = shared_from_this();
    // TODO: Initialize the slots.
    return result;
}

bool StructureTypeValue::isStructureTypeValue() const
{
    return true;
}

TypePtr StructureTypeValue::getType() const
{
    return type;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius