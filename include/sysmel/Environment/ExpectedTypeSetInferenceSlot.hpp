#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_EXPECTED_TYPE_SET_INFERENCE_SLOT_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_EXPECTED_TYPE_SET_INFERENCE_SLOT_HPP
#pragma once

#include "ResultTypeInferenceSlot.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ExpectedTypeSetInferenceSlot);

/**
 * I am the interface for an expected type inference slot. I am used for bidirectional propagation of the result type.
 */
class SYSMEL_COMPILER_LIB_EXPORT ExpectedTypeSetInferenceSlot : public SubtypeOf<ResultTypeInferenceSlot, ExpectedTypeSetInferenceSlot>
{
public:
    static constexpr char const __typeName__[] = "ExpectedTypeSetInferenceSlot";

    virtual ASTNodePtr concretizeTypeInferenceOfNodeWith(const ASTNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;
    virtual TypePtr getExpectedFunctionalArgumentType(size_t index) const override;
    virtual TypePtr getExpectedFunctionalResultType() const override;

    TypePtrList expectedTypeSet;
    bool isReceiverType = false;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_EXPECTED_TYPE_SET_RESULT_TYPE_INFERENCE_SLOT_HPP
