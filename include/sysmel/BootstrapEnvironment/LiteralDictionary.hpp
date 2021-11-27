#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_DICTIONARY_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_DICTIONARY_HPP
#pragma once

#include "LiteralValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralDictionary);


/**
 * I am literal dictionary value.
 */
class LiteralDictionary : public SubtypeOf<LiteralValue, LiteralDictionary>
{
public:
    static constexpr char const __typeName__[] = "LiteralDictionary";
    static constexpr char const __sysmelTypeName__[] = "LiteralDictionary";

    static LiteralDictionaryPtr makeFor(const AnyValuePtrList &elements);

    static MethodCategories __instanceMethods__();

    virtual bool isLiteralDictionary() const override;

    virtual AnyValuePtrList unwrapAsArray() const override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

protected:
    AnyValuePtrList elements;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_DICTIONARY_HPP