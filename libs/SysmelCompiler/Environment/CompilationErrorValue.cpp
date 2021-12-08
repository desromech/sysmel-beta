#include "Environment/CompilationErrorValue.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel