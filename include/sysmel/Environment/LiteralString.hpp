#ifndef SYSMEL_ENVIRONMENT_LITERAL_STRING_HPP
#define SYSMEL_ENVIRONMENT_LITERAL_STRING_HPP
#pragma once

#include "LiteralValue.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralString);

/**
 * I am a literal string value.
 */
class SYSMEL_COMPILER_LIB_EXPORT LiteralString : public SubtypeOf<LiteralValue, LiteralString>
{
public:
    static constexpr char const __typeName__[] = "LiteralString";
    static constexpr char const __sysmelTypeName__[] = "LiteralString";

    static LiteralStringPtr makeFor(const std::string &value);
    static MethodCategories __instanceMethods__();
        
    virtual bool isLiteralString() const override;
    virtual AnyValuePtr acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor) override;

    virtual std::string asString() const override;
    virtual std::string printString() const override;
    virtual std::string unwrapAsString() const override;
    virtual SExpression asSExpression() const override;
    
protected:
    std::string value;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_LITERAL_STRING_HPP