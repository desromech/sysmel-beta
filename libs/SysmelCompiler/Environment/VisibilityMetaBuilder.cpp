#include "Environment/VisibilityMetaBuilder.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

// Types
#include "Environment/EnumMetaBuilder.hpp"
#include "Environment/StructMetaBuilder.hpp"
#include "Environment/UnionMetaBuilder.hpp"
#include "Environment/ClassMetaBuilder.hpp"

// Functional
#include "Environment/FunctionMetaBuilder.hpp"
#include "Environment/MethodMetaBuilder.hpp"

// Variables
#include "Environment/FieldMetaBuilder.hpp"
#include "Environment/GlobalMetaBuilder.hpp"

// Meta
#include "Environment/CompileTimeMetaBuilder.hpp"
#include "Environment/TemplateMetaBuilder.hpp"

namespace Sysmel
{
namespace Environment
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

static std::unordered_map<std::string, MethodFlags> methodFlagsMap = {
    {"abstract", MethodFlags::Abstract},
    {"const", MethodFlags::Const},
    {"constructor", MethodFlags::Constructor},
    {"conversion", MethodFlags::Conversion},
    {"explicit", MethodFlags::Explicit},
    {"final", MethodFlags::Final},
    {"macro", MethodFlags::Macro},
    {"noThrow", MethodFlags::NoThrow},
    {"override", MethodFlags::Override},
    {"pure", MethodFlags::Pure},
    {"returnsTwice", MethodFlags::ReturnsTwice},
    {"static", MethodFlags::Static},
    {"virtual", MethodFlags::Virtual},
    {"notInCompiletime", MethodFlags::NotInCompileTime},
    {"inline", MethodFlags::Inline},
};

static std::unordered_map<std::string, CallingConvention> callingConventionMap = {
    {"cdecl", CallingConvention::Cdecl},
    {"stdcall", CallingConvention::Stdcall},
    {"thiscal", CallingConvention::Thiscall},
    {"apicall", CallingConvention::Apicall},
};

ASTNodePtr VisibilityMetaBuilder::analyzeMessageSendNodeWithSelector(const std::string &selector, const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(node->arguments.empty())
    {
        // Method flags.
        {
            auto it = methodFlagsMap.find(selector);
            if(it != methodFlagsMap.end())
            {
                instanceContext->methodFlags = instanceContext->methodFlags | it->second;
                return node->receiver;
            }
        }

        // Calling convention
        {
            auto it = callingConventionMap.find(selector);
            if(it != callingConventionMap.end())
            {
                instanceContext->callingConvention = it->second;
                return node->receiver;
            }
        }

        // Extern language.
        if(selector == "externC")
        {
            instanceContext->externalLanguageMode = ExternalLanguageMode::C;
            return node->receiver;
        }
        else if(selector == "externCpp")
        {
            instanceContext->externalLanguageMode = ExternalLanguageMode::Cpp;
            return node->receiver;
        }

        // Dll linkage
        if(selector == "dllexport")
        {
            instanceContext->dllLinkageMode = DllLinkageMode::Export;
            return node->receiver;
        }
        else if(selector == "dllimport")
        {
            instanceContext->dllLinkageMode = DllLinkageMode::Import;
            return node->receiver;
        }

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
        else if(selector == "template")
            return delegateToMetaBuilderAt<TemplateMetaBuilder> (node->sourcePosition);
    }

    return SuperType::analyzeMessageSendNodeWithSelector(selector, node, semanticAnalyzer);
}


} // End of namespace Environment
} // End of namespace Sysmel