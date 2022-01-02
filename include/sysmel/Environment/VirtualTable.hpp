#ifndef SYSMEL_ENVIRONMENT_VIRTUAL_TABLE_HPP
#define SYSMEL_ENVIRONMENT_VIRTUAL_TABLE_HPP
#pragma once

#include "PrimitiveType.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(VirtualTable);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(SpecificMethod);

/**
 * I am literal number value.
 */
class SYSMEL_COMPILER_LIB_EXPORT VirtualTable : public SubtypeOf<AnyValue, VirtualTable>
{
public:
    static constexpr char const __typeName__[] = "VirtualTable";

    virtual bool isVirtualTable() const override;
    virtual AnyValuePtr acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor) override;

    TypeWeakPtr ownerType;
    uint64_t receiverSlotIndex = 0;
    SpecificMethodPtrList slots;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_VIRTUAL_TABLE_HPP