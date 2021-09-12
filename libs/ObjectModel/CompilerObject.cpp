#include "sysmel/ObjectModel/CompilerObject.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<CompilerObject> compilerObjectTypeRegistration;

bool CompilerObject::isCompilerObject() const
{
    return true;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius