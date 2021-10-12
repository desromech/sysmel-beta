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

ResultTypeInferenceSlotPtr ResultTypeInferenceSlot::makeForTypeSet(const TypePtrList &expectedTypeSet)
{
    (void)expectedTypeSet;
    return std::make_shared<ResultTypeInferenceSlot> ();
}

ResultTypeInferenceSlotPtr ResultTypeInferenceSlot::makeForAuto()
{
    return std::make_shared<ResultTypeInferenceSlot> ();
}

ResultTypeInferenceSlotPtr ResultTypeInferenceSlot::makeForAutoWithMode(TypeInferenceMode mode, bool isMutable)
{
    (void)mode;
    (void)isMutable;
    return std::make_shared<ResultTypeInferenceSlot> ();
}

TypePtr ResultTypeInferenceSlot::getExpectedFunctionalArgumentType(size_t index) const
{
    (void)index;
    return nullptr;
}

TypePtr ResultTypeInferenceSlot::getExpectedFunctionalResultType() const
{
    return nullptr;
}


} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius