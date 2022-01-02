#ifndef SYSMEL_ENVIRONMENT_ITANIUM_NAME_MANGLER_HPP
#define SYSMEL_ENVIRONMENT_ITANIUM_NAME_MANGLER_HPP
#pragma once

#include "NameMangler.hpp"
#include <unordered_map>

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ItaniumNameMangler);

/**
 * I am the base interface for a meta-builder.
 */
class SYSMEL_COMPILER_LIB_EXPORT ItaniumNameMangler : public SubtypeOf<NameMangler, ItaniumNameMangler>
{
public:
    static constexpr char const __typeName__[] = "ItaniumNameMangler";

    ItaniumNameMangler();

    virtual std::string mangleType(const TypePtr &type) override;
    virtual std::string mangleTypeInfo(const TypePtr &type) override;
    virtual std::string mangleVirtualTable(const VirtualTablePtr &vtable) override;
    virtual std::string mangleProgramEntity(const ProgramEntityPtr &programEntity) override;
    virtual std::string mangleSSAProgramEntity(const SSAProgramEntityPtr &programEntity) override;

protected:
    friend class ItaniumNameManglerContext;
    std::unordered_map<TypePtr, std::string> primitiveTypeMap;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_ITANIUM_NAME_MANGLER_HPP
