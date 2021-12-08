#ifndef SYSMEL_ENVIRONMENT_LITERAL_ASSOCIATION_HPP
#define SYSMEL_ENVIRONMENT_LITERAL_ASSOCIATION_HPP
#pragma once

#include "LiteralValue.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralAssociation);


/**
 * I am literal association value.
 */
class LiteralAssociation : public SubtypeOf<LiteralValue, LiteralAssociation>
{
public:
    static constexpr char const __typeName__[] = "LiteralAssociation";
    static constexpr char const __sysmelTypeName__[] = "LiteralAssociation";

    static LiteralAssociationPtr makeWith(const AnyValuePtr &key, const AnyValuePtr &value);

    static FieldVariablePtrList __fieldVariables__();
    static MethodCategories __instanceMethods__();

    virtual bool isLiteralAssociation() const override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    const AnyValuePtr &getKey() const
    {
        return key;
    }

    const AnyValuePtr &getValue() const
    {
        return value;
    }

protected:
    AnyValuePtr key;
    AnyValuePtr value;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_LITERAL_ASSOCIATION_HPP