#include "sysmel/BootstrapEnvironment/ASTLexicalScopeNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTLexicalScopeNode> ASTLexicalScopeNodeTypeRegistration;

bool ASTLexicalScopeNode::isASTLexicalScopeNode() const
{
    return true;
}
AnyValuePtr ASTLexicalScopeNode::encodeAsSExpression() const
{
    return wrapValue(AnyValuePtrList{internSymbol("lexicalScope"), body ? body->encodeAsSExpression() : getNilConstant()});
}


} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius