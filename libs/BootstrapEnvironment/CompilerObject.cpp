#include "sysmel/BootstrapEnvironment/CompilerObject.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<CompilerObject> compilerObjectTypeRegistration;

bool CompilerObject::isCompilerObject() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius