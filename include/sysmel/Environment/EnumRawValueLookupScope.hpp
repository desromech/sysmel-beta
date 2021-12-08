#ifndef SYSMEL_ENVIRONMENT_ENUM_RAW_VALUE_LOOKUP_SCOPE_HPP
#define SYSMEL_ENVIRONMENT_ENUM_RAW_VALUE_LOOKUP_SCOPE_HPP
#pragma once

#include "IdentifierLookupScope.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(EnumRawValueLookupScope);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(EnumType);
/**
 * I am the interface for the scope that is used for the identifier lookup.
 */
class EnumRawValueLookupScope : public SubtypeOf<IdentifierLookupScope, EnumRawValueLookupScope>
{
public:
    static constexpr char const __typeName__[] = "EnumRawValueLookupScope";

    /// This method performs a symbol lookup locally.
    virtual AnyValuePtr lookupSymbolLocally(const AnyValuePtr &symbol) override;

    EnumTypePtr enumType;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_ENUM_RAW_VALUE_LOOKUP_SCOPE_HPP