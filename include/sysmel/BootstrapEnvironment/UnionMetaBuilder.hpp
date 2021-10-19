#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_UNION_METABUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_UNION_METABUILDER_HPP
#pragma once

#include "TypeMetaBuilder.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(UnionMetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class UnionMetaBuilder : public SubtypeOf<TypeMetaBuilder, UnionMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "UnionMetaBuilder";

protected:
    virtual ASTNodePtr concretizeMetaBuilder() override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_UNION_METABUILDER_HPP
