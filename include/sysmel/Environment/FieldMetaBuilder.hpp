#ifndef SYSMEL_ENVIRONMENT_FIELD_METABUILDER_HPP
#define SYSMEL_ENVIRONMENT_FIELD_METABUILDER_HPP
#pragma once

#include "VariableMetaBuilder.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(FieldMetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class FieldMetaBuilder : public SubtypeOf<VariableMetaBuilder, FieldMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "FieldMetaBuilder";


protected:
    virtual ASTNodePtr concretizeMetaBuilder() override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_FIELD_METABUILDER_HPP
