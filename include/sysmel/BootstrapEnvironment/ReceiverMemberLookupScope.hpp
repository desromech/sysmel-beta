#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_RECEIVER_MEMBER_LOOKUP_SCOPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_RECEIVER_MEMBER_LOOKUP_SCOPE_HPP
#pragma once

#include "IdentifierLookupScope.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ReceiverMemberLookupScope);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(Variable);

/**
 * I am an lookup scope that is used for accessing the implicitly imported receiver fields and member functions.
 */
class ReceiverMemberLookupScope : public SubtypeOf<IdentifierLookupScope, ReceiverMemberLookupScope>
{
public:
    static constexpr char const __typeName__[] = "ReceiverMemberLookupScope";

    /// This method performs a symbol lookup locally.
    virtual AnyValuePtr lookupSymbolLocally(const AnyValuePtr &symbol) override;

    TypePtr receiverType;
    VariablePtr receiverVariable;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_RECEIVER_MEMBER_LOOKUP_SCOPE_HPP