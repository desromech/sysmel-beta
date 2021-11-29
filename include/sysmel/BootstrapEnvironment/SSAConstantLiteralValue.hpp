#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CONSTANT_LITERAL_VALUE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CONSTANT_LITERAL_VALUE_HPP
#pragma once

#include "SSAConstantValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAConstantLiteralValue : public SubtypeOf<SSAConstantValue, SSAConstantLiteralValue>
{
public:
    static constexpr char const __typeName__[] = "SSAConstantLiteralValue";

    static SSAConstantLiteralValuePtr makeWith(const AnyValuePtr &value, const TypePtr &valueType);

    virtual bool isSSAConstantLiteralValue() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;
    virtual SExpression asFullSExpression() const override;

    virtual TypePtr getValueType() const override;
    const AnyValuePtr &getValue() const;

protected:
    AnyValuePtr value;
    TypePtr valueType;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CONSTANT_LITERAL_VALUE_HPP