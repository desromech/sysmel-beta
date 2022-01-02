#ifndef SYSMEL_ENVIRONMENT_SSA_GLOBAL_VARIABLE_HPP
#define SYSMEL_ENVIRONMENT_SSA_GLOBAL_VARIABLE_HPP
#pragma once

#include "SSAProgramEntity.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SYSMEL_COMPILER_LIB_EXPORT SSAGlobalVariable : public SubtypeOf<SSAProgramEntity, SSAGlobalVariable>
{
public:
    static constexpr char const __typeName__[] = "SSAGlobalVariable";

    virtual bool isSSAGlobalVariable() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual SExpression asSExpression() const override;
    virtual SExpression asFullDefinitionSExpression() const override;

    TypePtr getValueType() const override;
    TypePtr getContentType() const;
    void setContentAndValueType(const TypePtr &newContentType, const TypePtr &newValueType);

    const AnyValuePtr &getInitialValue();
    void setInitialValue(const AnyValuePtr &newInitialValue);

    bool isExternallyDefined() const;
    void setExternallyDefined(bool newExternallyDefined);

private:
    TypePtr valueType;
    TypePtr contentType;
    AnyValuePtr initialValue;
    bool externallyDefined = false;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_GLOBAL_VARIABLE_HPP