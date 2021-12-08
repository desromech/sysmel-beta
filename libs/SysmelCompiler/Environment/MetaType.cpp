#include "Environment/MetaType.hpp"
#include "Environment/ASTCallNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<MetaType> metaTypeTypeRegistration;

ASTNodePtr MetaType::analyzeCallNode(const ASTCallNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    return extractTypeForTypeMacroReceiverNode(node->function)->analyzeValueConstructionWithArguments(node, node->arguments, semanticAnalyzer);
}

void MetaType::setThisType(const TypePtr &instanceType)
{
    thisType = instanceType;
}

bool MetaType::isMetaType() const
{
    return true;
}

TypePtr MetaType::getInstanceType()
{
    return thisType;
}

TypePtr MetaType::getMetaType()
{
    return selfFromThis();
}

std::string MetaType::printString() const
{
    return thisType->printString() + " __type__";
}

} // End of namespace Environment
} // End of namespace Sysmel