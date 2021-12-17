#ifndef SYSMEL_ENVIRONMENT_LITERAL_BOOLEAN_HPP
#define SYSMEL_ENVIRONMENT_LITERAL_BOOLEAN_HPP
#pragma once

#include "LiteralValue.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralBoolean);

/**
 * I am literal number value.
 */
class LiteralBoolean : public SubtypeOf<LiteralValue, LiteralBoolean>
{
public:
    static constexpr char const __typeName__[] = "LiteralBoolean";
    static constexpr char const __sysmelTypeName__[] = "LiteralBoolean";

    virtual bool isLiteralBoolean() const override;
    virtual AnyValuePtr acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor) override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_LITERAL_BOOLEAN_HPP