#include "sysmel/BootstrapEnvironment/CompilationErrorValue.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<CompilationErrorValue> compilationErrorValueRegistration;

TypePtr Type::getCompilationErrorValueType()
{
    return CompilationErrorValue::__staticType__();
}

AnyValuePtr getCompilationErrorValueConstant()
{
    return CompilationErrorValue::uniqueInstance();
}

CompilationErrorValuePtr CompilationErrorValue::uniqueInstance()
{
    static auto singleton = basicMakeGlobalSingletonObject<CompilationErrorValue> ();
    return singleton;
}

bool CompilationErrorValue::isCompilationErrorValue() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius