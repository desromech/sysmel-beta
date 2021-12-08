#include "Environment/CompilerObject.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<CompilerObject> compilerObjectTypeRegistration;

bool CompilerObject::isCompilerObject() const
{
    return true;
}

} // End of namespace Environment
} // End of namespace Sysmel