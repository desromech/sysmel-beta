#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CLASS_METABUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CLASS_METABUILDER_HPP
#pragma once

#include "TypeMetaBuilder.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ClassMetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class ClassMetaBuilder : public SubtypeOf<TypeMetaBuilder, ClassMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "ClassMetaBuilder";

//protected:
//    virtual ASTNodePtr concretizeMetaBuilder() override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CLASS_METABUILDER_HPP
