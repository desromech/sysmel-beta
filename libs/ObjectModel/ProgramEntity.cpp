#include "sysmel/ObjectModel/ProgramEntity.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<ProgramEntity> programEntityTypeRegistration;

ModulePtr ProgramEntity::getDefinitionModule() const
{
    return ModulePtr();
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius