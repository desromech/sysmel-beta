#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_INTERNAL_METABUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_INTERNAL_METABUILDER_HPP
#pragma once

#include "VisibilityMetaBuilder.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LetMetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class InternalMetaBuilder : public SubtypeOf<VisibilityMetaBuilder, InternalMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "InternalMetaBuilder";

    virtual ProgramEntityVisibility getVisibility() const;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_INTERNAL_METABUILDER_HPP