#ifndef SYSMEL_ENVIRONMENT_MACRO_INVOCATION_CONTEXT_HPP
#define SYSMEL_ENVIRONMENT_MACRO_INVOCATION_CONTEXT_HPP
#pragma once

#include "CompilerObject.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(MacroInvocationContext);

/**
 * I am the base interface for compile time object that is passed through the interpreter.
 */
class SYSMEL_COMPILER_LIB_EXPORT MacroInvocationContext : public SubtypeOf<CompilerObject, MacroInvocationContext>
{
public:
    static constexpr char const __typeName__[] = "MacroInvocationContext";

    static FieldVariablePtrList __fieldVariables__();
    
    virtual bool isMacroInvocationContext() const override;

    ASTNodePtr receiverNode;
    TypePtr selfType;
    ASTBuilderPtr astBuilder;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_LITERAL_VALUE_HPP