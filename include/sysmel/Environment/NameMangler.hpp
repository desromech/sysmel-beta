#ifndef SYSMEL_ENVIRONMENT_NAME_MANGLER_HPP
#define SYSMEL_ENVIRONMENT_NAME_MANGLER_HPP
#pragma once

#include "CompilerObject.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(NameMangler);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ProgramEntity);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAProgramEntity);

/**
 * I am the base interface for a meta-builder.
 */
class NameMangler : public SubtypeOf<CompilerObject, NameMangler>
{
public:
    static constexpr char const __typeName__[] = "NameMangler";

    virtual std::string mangleType(const TypePtr &type);
    virtual std::string mangleTypeInfo(const TypePtr &type);
    virtual std::string mangleProgramEntity(const ProgramEntityPtr &programEntity);
    virtual std::string mangleSSAProgramEntity(const SSAProgramEntityPtr &programEntity);
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_NAMED_PROGRAM_ENTITY_METABUILDER_HPP
