#ifndef SYSMEL_ENVIRONMENT_INTERNAL_METABUILDER_HPP
#define SYSMEL_ENVIRONMENT_INTERNAL_METABUILDER_HPP
#pragma once

#include "VisibilityMetaBuilder.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(InternalMetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class SYSMEL_COMPILER_LIB_EXPORT InternalMetaBuilder : public SubtypeOf<VisibilityMetaBuilder, InternalMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "InternalMetaBuilder";

    virtual ProgramEntityVisibility getVisibility() const;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_INTERNAL_METABUILDER_HPP
