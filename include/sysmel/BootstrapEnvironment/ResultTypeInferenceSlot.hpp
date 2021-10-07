#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_RESULT_TYPE_INFERENCE_SLOT_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_RESULT_TYPE_INFERENCE_SLOT_HPP
#pragma once

#include "CompilerObject.hpp"
#include "TypeInferenceMode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ResultTypeInferenceSlot);

/**
 * I am the interface for an expected type inference slot. I am used for bidirectional propagation of the result type.
 */
class ResultTypeInferenceSlot : public SubtypeOf<CompilerObject, ResultTypeInferenceSlot>
{
public:
    static constexpr char const __typeName__[] = "ResultTypeInferenceSlot";

    static ResultTypeInferenceSlotPtr makeForType(const TypePtr &expectedType);
    static ResultTypeInferenceSlotPtr makeForTypeSet(const TypePtrList &expectedTypeSet);
    static ResultTypeInferenceSlotPtr makeForAuto();
    static ResultTypeInferenceSlotPtr makeForAutoWithMode(TypeInferenceMode mode, bool isMutable);
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_RESULT_TYPE_INFERENCE_SLOT_HPP
