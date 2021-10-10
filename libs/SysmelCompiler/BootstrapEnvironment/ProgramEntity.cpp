#include "sysmel/BootstrapEnvironment/ProgramEntity.hpp"
#include "sysmel/BootstrapEnvironment/UnsupportedOperation.hpp"
#include "sysmel/BootstrapEnvironment/Module.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/StringUtilities.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

void ProgramEntity::recordChildProgramEntityDefinition(const ProgramEntityPtr &newChild)
{
    signalNewWithMessage<UnsupportedOperation> (formatString("Cannot record child program {0} entity inside of {1}.",
        {{newChild->printString(), printString()}}));
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius
