#ifndef SYSMEL_ENVIRONMENT_UNION_METABUILDER_HPP
#define SYSMEL_ENVIRONMENT_UNION_METABUILDER_HPP
#pragma once

#include "TypeMetaBuilder.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_UNION_METABUILDER_HPP
