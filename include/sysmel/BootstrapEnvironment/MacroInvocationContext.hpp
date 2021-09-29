#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_MACRO_INVOCATION_CONTEXT_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_MACRO_INVOCATION_CONTEXT_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(MacroInvocationContext);

/**
 * I am the base interface for compile time object that is passed through the interpreter.
 */
class MacroInvocationContext : public SubtypeOf<CompilerObject, MacroInvocationContext>
{
public:
    static constexpr char const __typeName__[] = "MacroInvocationContext";

    virtual bool isMacroInvocationContext() const override;

    ASTNodePtr receiverNode;
    TypePtr selfType;
    ASTBuilderPtr astBuilder;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LITERAL_VALUE_HPP