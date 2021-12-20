#ifndef SYSMEL_ENVIRONMENT_SSA_LOCAL_VARIABLE_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_LOCAL_VARIABLE_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSALocalVariableInstruction : public SubtypeOf<SSAInstruction, SSALocalVariableInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSALocalVariableInstruction";

    virtual bool isSSALocalVariableInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual TypePtr getValueType() const override;

    virtual std::string getMnemonic() const override;

    const TypePtr &getVariableValueType() const;
    void setVariableValueType(const TypePtr &newValueType);

    const TypePtr &getVariableReferenceType() const;
    void setVariableReferenceType(const TypePtr &newReferenceType);

    bool isLocalFinalizationRequired() const;
    virtual void markLocalFinalizationRequired() override;
    
protected:
    TypePtr referenceType;
    TypePtr variableValueType;

    bool localFinalizationRequired = false;

};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_LOCAL_VARIABLE_INSTRUCTION_HPP