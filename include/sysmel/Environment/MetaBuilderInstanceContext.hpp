#ifndef SYSMEL_ENVIRONMENT_META_BUILDER_INSTANCE_CONTEXT_HPP
#define SYSMEL_ENVIRONMENT_META_BUILDER_INSTANCE_CONTEXT_HPP
#pragma once

#include "CompilerObject.hpp"
#include "ProgramEntityVisibility.hpp"
#include "MethodFlags.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_META_BUILDER_INSTANCE_CONTEXT_HPP