#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_META_BUILDER_INSTANCE_CONTEXT_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_META_BUILDER_INSTANCE_CONTEXT_HPP
#pragma once

#include "CompilerObject.hpp"
#include "ProgramEntityVisibility.hpp"
#include "MethodFlags.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(MetaBuilderInstanceContext);

/**
 * I am the base interface for compile time object that is passed through the interpreter.
 */
class MetaBuilderInstanceContext : public SubtypeOf<CompilerObject, MetaBuilderInstanceContext>
{
public:
    static constexpr char const __typeName__[] = "MetaBuilderInstanceContext";

    ASTSourcePositionPtr concreteSourcePosition();

    ASTNodePtr instanceNode;

    ProgramEntityVisibility programEntityVisibility = ProgramEntityVisibility::Default;
    MethodFlags methodFlags = MethodFlags::None;
    bool isCompileTime = false;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_META_BUILDER_INSTANCE_CONTEXT_HPP