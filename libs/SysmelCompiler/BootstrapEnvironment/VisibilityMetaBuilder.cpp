#include "sysmel/BootstrapEnvironment/VisibilityMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

// Types
#include "sysmel/BootstrapEnvironment/EnumMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/StructMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/UnionMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ClassMetaBuilder.hpp"

// Functional
#include "sysmel/BootstrapEnvironment/FunctionMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/MethodMetaBuilder.hpp"

// Variables
#include "sysmel/BootstrapEnvironment/FieldMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/GlobalMetaBuilder.hpp"

// Meta
#include "sysmel/BootstrapEnvironment/CompileTimeMetaBuilder.hpp"


namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<VisibilityMetaBuilder> visibilityMetaBuilderTypeRegistration;

void VisibilityMetaBuilder::setMetaBuilderInstanceContext(const MetaBuilderInstanceContextPtr &context)
{
    SuperType::setMetaBuilderInstanceContext(context);
    context->programEntityVisibility = getVisibility();
}

ProgramEntityVisibility VisibilityMetaBuilder::getVisibility() const
{
    return ProgramEntityVisibility::Default;
}

ASTNodePtr VisibilityMetaBuilder::analyzeMessageSendNodeWithSelector(const std::string &selector, const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(node->arguments.empty())
    {
        // Functional
        if(selector == "function")
            return delegateToMetaBuilderAt<FunctionMetaBuilder> (node->sourcePosition);
        else if(selector == "method")
            return delegateToMetaBuilderAt<MethodMetaBuilder> (node->sourcePosition);

        // Types
        else if(selector == "enum")
            return delegateToMetaBuilderAt<EnumMetaBuilder> (node->sourcePosition);
        else if(selector == "struct")
            return delegateToMetaBuilderAt<StructMetaBuilder> (node->sourcePosition);
        else if(selector == "union")
            return delegateToMetaBuilderAt<UnionMetaBuilder> (node->sourcePosition);
        else if(selector == "class")
            return delegateToMetaBuilderAt<ClassMetaBuilder> (node->sourcePosition);

        // Variables
        else if(selector == "field")
            return delegateToMetaBuilderAt<FieldMetaBuilder> (node->sourcePosition);
        else if(selector == "global")
            return delegateToMetaBuilderAt<GlobalMetaBuilder> (node->sourcePosition);

        // Meta
        else if(selector == "compileTime")
            return delegateToMetaBuilderAt<CompileTimeMetaBuilder> (node->sourcePosition);
    }

    return SuperType::analyzeMessageSendNodeWithSelector(selector, node, semanticAnalyzer);
}


} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius