#include "sysmel/ObjectModel/TranslationUnitModule.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{
static BootstrapTypeRegistration<TranslationUnitModule> translationUnitModuleTypeRegistration;

bool TranslationUnitModule::isTranslationUnitModule() const
{
    return true;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius