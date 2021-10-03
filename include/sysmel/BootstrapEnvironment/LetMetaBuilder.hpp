#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LET_METABUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LET_METABUILDER_HPP
#pragma once

#include "MetaBuilder.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(MetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class LetMetaBuilder : public SubtypeOf<MetaBuilder, LetMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "LetMetaBuilder";
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LET_METABUILDER_HPP
