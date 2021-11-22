#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_EXPECTED_AUTO_TYPE_INFERENCE_SLOT_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_EXPECTED_AUTO_TYPE_INFERENCE_SLOT_HPP
#pragma once

#include "ResultTypeInferenceSlot.hpp"
#include "TypeInferenceMode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(AutoTypeInferenceSlot);

/**
 * I am the interface for an expected type inference slot. I am used for bidirectional propagation of the result type.
 */
class AutoTypeInferenceSlot : public SubtypeOf<ResultTypeInferenceSlot, AutoTypeInferenceSlot>
{
public:
    static constexpr char const __typeName__[] = "AutoTypeInferenceSlot";

    virtual ASTNodePtr concretizeTypeInferenceOfNodeWith(const ASTNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    TypeInferenceMode mode = TypeInferenceMode::Value;
    bool isMutable = false;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_EXPECTED_TYPE_SET_RESULT_TYPE_INFERENCE_SLOT_HPP
