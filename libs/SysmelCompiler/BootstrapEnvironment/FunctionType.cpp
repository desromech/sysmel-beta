#include "sysmel/BootstrapEnvironment/FunctionType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<FunctionTypeValue> functionalTypeRegistration;

FunctionTypePtr FunctionType::makeForMethodSignature(const MethodSignature &signature)
{
    auto result = std::make_shared<FunctionType> ();
    result->arguments = signature.argumentTypes;
    result->result = signature.resultType;
    return result;
}

bool FunctionType::isFunctionType() const
{
    return true;
}

bool FunctionTypeValue::isFunctionTypeValue() const
{
    return true;
}

TypePtr FunctionTypeValue::getType() const
{
    return type;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius