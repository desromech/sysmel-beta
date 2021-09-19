#include "sysmel/BootstrapEnvironment/TranslationUnitModule.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<TranslationUnitModule> translationUnitModuleTypeRegistration;

bool TranslationUnitModule::isTranslationUnitModule() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius