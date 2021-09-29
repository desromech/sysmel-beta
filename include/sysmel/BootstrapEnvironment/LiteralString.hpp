#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_STRING_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_STRING_HPP
#pragma once

#include "LiteralValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralString);

/**
 * I am a literal string value.
 */
class LiteralString : public SubtypeOf<LiteralValue, LiteralString>
{
public:
    static constexpr char const __typeName__[] = "LiteralString";

    static LiteralStringPtr makeFor(const std::string &value);
        
    virtual bool isLiteralString() const override;

    virtual std::string asString() const override;
    virtual std::string printString() const override;
    virtual std::string unwrapAsString() const override;
    virtual SExpression asSExpression() const override;
    
protected:
    std::string value;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_STRING_HPP