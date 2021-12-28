#ifndef SYSMEL_ENVIRONMENT_PRIMITIVE_BOOLEAN_TYPE_HPP
#define SYSMEL_ENVIRONMENT_PRIMITIVE_BOOLEAN_TYPE_HPP
#pragma once

#include "PrimitiveScalarType.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(PrimitiveBooleanType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(Boolean8);

/**
 * I am a primitive boolean type.
 */
class SYSMEL_COMPILER_LIB_EXPORT PrimitiveBooleanType : public SubtypeOf<PrimitiveScalarType, PrimitiveBooleanType>
{
public:
    static constexpr char const __typeName__[] = "PrimitiveBooleanType";
    static constexpr char const __sysmelTypeName__[] = "_PrimitiveBooleanType";

    virtual bool isPrimitiveBooleanTypeValue() const override;
    virtual AnyValuePtr acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor) override;
};

/**
 * I am a 8 bits boolean value.
 */
class SYSMEL_COMPILER_LIB_EXPORT Boolean8 : public SubtypeOf<PrimitiveBooleanType, Boolean8>
{
public:
    typedef bool ValueType;
    
    static constexpr char const __typeName__[] = "Boolean8";
    static constexpr char const __sysmelTypeName__[] = "Boolean8";

    static Boolean8Ptr make(bool value);

    static MethodCategories __instanceMethods__();
    static MethodCategories __typeMacroMethods__();
    static AnyValuePtrList __constructors__();
    static bool __canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value);
    static AnyValuePtr __instantiateWithLiteralValue__(const AnyValuePtr &value);
    static constexpr uint64_t __memorySize__ = 1;
    static constexpr uint64_t __memoryAlignment__()
    {
        return 1;
    }
    
    virtual bool unwrapAsBoolean() const override;

    virtual std::string asString() const override;
    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    bool value = false;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_PRIMITIVE_BOOLEAN_TYPE_HPP