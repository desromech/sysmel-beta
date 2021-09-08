#include "sysmel/ObjectModel/ProgramEntity.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<ProgramEntity> programEntityTypeRegistration;

bool ProgramEntity::isProgramEntity() const
{
    return true;
}

ModulePtr ProgramEntity::getDefinitionModule() const
{
    return ModulePtr();
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius