#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CONSTANT_LITERAL_VALUE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CONSTANT_LITERAL_VALUE_HPP
#pragma once

#include "SSAConstantValue.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAConstantLiteralValue : public SubtypeOf<SSAConstantValue, SSAConstantLiteralValue>
{
public:
    static constexpr char const __typeName__[] = "SSAConstantLiteralValue";

    static SSAConstantLiteralValuePtr makeWith(const AnyValuePtr &value, const TypePtr &valueType, const ASTSourcePositionPtr &sourcePosition);

    virtual bool isSSAConstantLiteralValue() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    virtual TypePtr getValueType() const override;
    const AnyValuePtr &getValue() const;

protected:
    AnyValuePtr value;
    TypePtr valueType;
    ASTSourcePositionPtr sourcePosition;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CONSTANT_LITERAL_VALUE_HPP