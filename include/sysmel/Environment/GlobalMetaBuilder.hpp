#ifndef SYSMEL_ENVIRONMENT_GLOBAL_METABUILDER_HPP
#define SYSMEL_ENVIRONMENT_GLOBAL_METABUILDER_HPP
#pragma once

#include "VariableMetaBuilder.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(GlobalMetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class GlobalMetaBuilder : public SubtypeOf<VariableMetaBuilder, GlobalMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "GlobalMetaBuilder";


protected:
    virtual ASTNodePtr concretizeMetaBuilder() override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_GLOBAL_METABUILDER_HPP
