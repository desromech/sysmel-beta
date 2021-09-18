#include "sysmel/ObjectModel/ASTLexicalScopeNode.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
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


} // End of namespace ObjectModel
} // End of namespace SysmelMoebius