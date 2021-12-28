#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_RESULT_TYPE_INFERENCE_SLOT_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_RESULT_TYPE_INFERENCE_SLOT_HPP
#pragma once

#include "CompilerObject.hpp"
#include "TypeInferenceMode.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ResultTypeInferenceSlot);

/**
 * I am the interface for an expected type inference slot. I am used for bidirectional propagation of the result type.
 */
class SYSMEL_COMPILER_LIB_EXPORT ResultTypeInferenceSlot : public SubtypeOf<CompilerObject, ResultTypeInferenceSlot>
{
public:
    static constexpr char const __typeName__[] = "ResultTypeInferenceSlot";

    static ResultTypeInferenceSlotPtr makeForType(const TypePtr &expectedType);
    static ResultTypeInferenceSlotPtr makeForReceiverType(const TypePtr &expectedType);
    static ResultTypeInferenceSlotPtr makeForTypeSet(const TypePtrList &expectedTypeSet);
    static ResultTypeInferenceSlotPtr makeForAuto();
    static ResultTypeInferenceSlotPtr makeForLiteralAuto();
    static ResultTypeInferenceSlotPtr makeForTemporaryAuto();
    static ResultTypeInferenceSlotPtr makeForAutoWithMode(TypeInferenceMode mode, bool isMutable, bool concreteLiterals);

    virtual ASTNodePtr concretizeTypeInferenceOfNodeWith(const ASTNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer);
    virtual TypePtr getExpectedFunctionalArgumentType(size_t index) const;
    virtual TypePtr getExpectedFunctionalResultType() const;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_RESULT_TYPE_INFERENCE_SLOT_HPP
