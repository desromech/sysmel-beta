#ifndef SYSMEL_ENVIRONMENT_VOID_HPP
#define SYSMEL_ENVIRONMENT_VOID_HPP
#pragma once

#include "AnyValue.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(Void);

/**
 * I am literal number value.
 */
class SYSMEL_COMPILER_LIB_EXPORT Void : public SubtypeOf<AnyValue, Void>
{
public:
    static constexpr char const __typeName__[] = "Void";
    static constexpr char const __sysmelTypeName__[] = "Void";
    static constexpr bool __isDynamicCompileTimeType__ = false;
    static constexpr bool __isNullableType__ = false;
    
    static VoidPtr uniqueInstance();

    virtual bool isVoid() const override;
    virtual AnyValuePtr acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor) override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_VOID_HPP