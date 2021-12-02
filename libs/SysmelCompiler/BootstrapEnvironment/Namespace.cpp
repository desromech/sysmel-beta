#include "sysmel/BootstrapEnvironment/Namespace.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/MethodDictionary.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/Error.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<Namespace> namespaceTypeRegistration;

TypePtr Type::getNamespaceType()
{
    return Namespace::__staticType__();
}

NamespacePtr Namespace::makeWithName(const AnyValuePtr &name)
{
    auto result = basicMakeObject<Namespace> ();
    result->name = name;
    return result;
}

bool Namespace::isNamespace() const
{
    return true;
}

AnyValuePtr Namespace::getName() const
{
    return name;
}

void Namespace::recordChildProgramEntityDefinition(const ProgramEntityPtr &newChild)
{
    children.push_back(newChild);
}

void Namespace::bindSymbolWithVisibility(const AnyValuePtr &symbol, ProgramEntityVisibility visibility, const ProgramEntityPtr &binding)
{
    assert(symbol && !symbol->isAnonymousNameSymbol());
    if(bindings.find(symbol) != bindings.end())
        signalNewWithMessage<Error> ("Expected a new symbol binding.");

    bindings[symbol] = std::make_pair(visibility, binding);
}

ASTNodePtr Namespace::analyzeMessageSendNode(const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(node->selector->isASTLiteralValueNode())
    {
        auto directSelectorValue = node->selector.staticAs<ASTLiteralValueNode> ()->value;

        // Attempt going through the different expansion levels
        while(uint8_t(node->expansionLevel) < uint8_t(MessageSendExpansionLevel::Count))
        {
            auto macroOrMethod = lookupSelectorInReceiverNodeWithExpansionLevel(directSelectorValue, node->receiver, node->expansionLevel);
            if(macroOrMethod)
                return macroOrMethod->analyzeMessageSendNode(node, semanticAnalyzer);

            node->expansionLevel = MessageSendExpansionLevel(uint8_t(node->expansionLevel) + 1);
        }

    }
    return SuperType::analyzeMessageSendNode(node, semanticAnalyzer);
}

AnyValuePtr Namespace::lookupExportedSymbolFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope)
{
    auto it = bindings.find(symbol);
    if(it == bindings.end())
        return nullptr;

    auto &binding = it->second;
    switch(binding.first)
    {
    case ProgramEntityVisibility::Public:
        return binding.second;
    default:
        return nullptr;
    }
}

AnyValuePtr Namespace::lookupLocalSymbolFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope)
{
    auto it = bindings.find(symbol);
    return it != bindings.end() ? it->second.second : nullptr;
}

AnyValuePtr Namespace::lookupLocalSelector(const AnyValuePtr &selector)
{
    return methodDictionary ? methodDictionary->lookupSelector(selector) : AnyValuePtr();
}

AnyValuePtr Namespace::lookupLocalMacroSelector(const AnyValuePtr &selector)
{
    return macroMethodDictionary ? macroMethodDictionary->lookupSelector(selector) : AnyValuePtr();
}

AnyValuePtr Namespace::lookupLocalMacroFallbackSelector(const AnyValuePtr &selector)
{
    return macroFallbackMethodDictionary ? macroFallbackMethodDictionary->lookupSelector(selector) : AnyValuePtr();
}

void Namespace::addMacroMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector)
{
    if(!macroMethodDictionary)
        macroMethodDictionary = basicMakeObject<MethodDictionary> ();
    macroMethodDictionary->addMethodWithSelector(method, selector);
}

void Namespace::addMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector)
{
    if(!methodDictionary)
        methodDictionary = basicMakeObject<MethodDictionary> ();

    methodDictionary->addMethodWithSelector(method, selector);
}

void Namespace::addMacroFallbackMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector)
{
    if(!macroMethodDictionary)
        macroMethodDictionary = basicMakeObject<MethodDictionary> ();
    macroMethodDictionary->addMethodWithSelector(method, selector);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius