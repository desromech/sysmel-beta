#include "sysmel/BootstrapEnvironment/ClosureType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ClosureTypeValue> functionalTypeRegistration;

ClosureTypePtr ClosureType::make(const TypePtr &resultType, const TypePtrList &arguments)
{
    auto result = std::make_shared<ClosureType> ();
    result->arguments = arguments;
    result->result = resultType;
    result->setSupertypeAndImplicitMetaType(ClosureTypeValue::__staticType__());
    return result;
}

ClosureTypePtr getOrCreateClosureType(const TypePtr &resultType, const TypePtrList &arguments)
{
    return ClosureType::make(resultType, arguments);
}

bool ClosureType::isClosureType() const
{
    return true;
}

std::string ClosureType::printString() const
{
    return "(" + SuperType::printString() + ") closure";
}

bool ClosureTypeValue::isClosureTypeValue() const
{
    return true;
}

TypePtr ClosureTypeValue::getType() const
{
    return type;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius