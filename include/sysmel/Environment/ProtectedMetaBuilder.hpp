#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROTECTED_METABUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROTECTED_METABUILDER_HPP
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
class ProtectedMetaBuilder : public SubtypeOf<VisibilityMetaBuilder, ProtectedMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "ProtectedMetaBuilder";

    virtual ProgramEntityVisibility getVisibility() const;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROTECTED_METABUILDER_HPP
