#include "sysmel/BootstrapEnvironment/MacroInvocationContext.hpp"
#include "sysmel/BootstrapEnvironment/ASTNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTBuilder.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapFieldVariable.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"


namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<MacroInvocationContext> macroInvocationContextRegistration;

FieldVariablePtrList MacroInvocationContext::__fieldVariables__()
{
    return FieldVariablePtrList{
        makeBootstrapFieldVariable("self", &SelfType::receiverNode),
        makeBootstrapFieldVariable("SelfType", &SelfType::selfType),
        makeBootstrapFieldVariable("__astBuilder", &SelfType::astBuilder),
    };
}

bool MacroInvocationContext::isMacroInvocationContext() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius