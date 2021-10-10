#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_VARIABLE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_VARIABLE_HPP
#pragma once

#include "ModuleDefinedProgramEntity.hpp"
#include "TypeInferenceMode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a variable program entity.
 */
class Variable : public SubtypeOf<ModuleDefinedProgramEntity, Variable>
{
public:
    static constexpr char const __typeName__[] = "Variable";

    virtual bool isVariable() const override;

    virtual void setDefinitionParameters(const AnyValuePtr &definitionName, const TypePtr &definitionValueType,
        TypeInferenceMode typeInferenceMode, bool definitionMutability, uint64_t definitionMinimalAlignment = 0);

    const AnyValuePtr &getName() const
    {
        return name;
    }

    const TypePtr &getValueType() const
    {
        return valueType;
    }

    const TypePtr &getReferenceType() const
    {
        return referenceType;
    }

    bool isMutable() const
    {
        return isMutable_;
    }

protected:
    AnyValuePtr name;
    TypePtr valueType;
    TypePtr referenceType;
    bool isMutable_ = false;
    uint64_t minimalAlignment = 0;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_VARIABLE_HPP