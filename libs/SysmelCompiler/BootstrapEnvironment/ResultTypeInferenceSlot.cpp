#include "sysmel/BootstrapEnvironment/ResultTypeInferenceSlot.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ResultTypeInferenceSlot> ResultTypeInferenceSlotTypeRegistration;

ResultTypeInferenceSlotPtr ResultTypeInferenceSlot::makeForType(const TypePtr &expectedType)
{
    (void)expectedType;
    return std::make_shared<ResultTypeInferenceSlot> ();
}

ResultTypeInferenceSlotPtr ResultTypeInferenceSlot::makeForAuto()
{
    return std::make_shared<ResultTypeInferenceSlot> ();
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius