#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_ARRAY_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_ARRAY_HPP
#pragma once

#include "LiteralValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralArray);


/**
 * I am literal number value.
 */
class LiteralArray : public SubtypeOf<LiteralValue, LiteralArray>
{
public:
    static constexpr char const __typeName__[] = "LiteralArray";
    static constexpr char const __sysmelTypeName__[] = "LiteralArray";

    static LiteralArrayPtr makeFor(const AnyValuePtrList &content);

    static MethodCategories __instanceMethods__();

    virtual bool isLiteralArray() const override;

    virtual AnyValuePtrList unwrapAsArray() const override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    LiteralArrayPtr select(const AnyValuePtr &aBlock) const;
    LiteralArrayPtr collect(const AnyValuePtr &aBlock) const;

protected:
    AnyValuePtrList content;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_ARRAY_HPP