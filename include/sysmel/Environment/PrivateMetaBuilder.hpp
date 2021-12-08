#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PRIVATE_METABUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PRIVATE_METABUILDER_HPP
#pragma once

#include "VisibilityMetaBuilder.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LetMetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class PrivateMetaBuilder : public SubtypeOf<VisibilityMetaBuilder, PrivateMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "PrivateMetaBuilder";

    virtual ProgramEntityVisibility getVisibility() const;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PRIVATE_METABUILDER_HPP
