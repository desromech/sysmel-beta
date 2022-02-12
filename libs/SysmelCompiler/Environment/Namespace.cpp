#include "Environment/Namespace.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/Type.hpp"
#include "Environment/MethodDictionary.hpp"
#include "Environment/IdentifierLookupScope.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/Error.hpp"
#include "Environment/SSANamespace.hpp"

namespace Sysmel
{
namespace Environment
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

std::string Namespace::getQualifiedName() const
{
    auto parent = getParentProgramEntity();
    if(parent && parent->isModule())
        return parent->getQualifiedName();
    return SuperType::getQualifiedName();
}

void Namespace::recordChildProgramEntityDefinition(const ProgramEntityPtr &newChild)
{
    children.push_back(newChild);
    newChild->setParentProgramEntity(selfFromThis());
}

void Namespace::bindSymbolWithVisibility(const AnyValuePtr &symbol, ProgramEntityVisibility symbolVisibility, const ProgramEntityPtr &binding)
{
    sysmelAssert(symbol && !symbol->isAnonymousNameSymbol());
    ProgramEntityPtr newBinding = binding;
    {
        auto existentIt = bindings.find(symbol);
        if(existentIt != bindings.end())
        {
            auto existentBinding = existentIt->second;
            if(binding->canOverloadBinding(existentBinding.second))
                newBinding = binding->makeOverloadedBindingWith(existentBinding.second);
            else
                signalNewWithMessage<Error> ("Expected a new symbol binding.");
        }
    }

    bindings[symbol] = std::make_pair(symbolVisibility, newBinding);

    if(symbol->isLiteralSymbol() && symbolVisibility == ProgramEntityVisibility::Public)
        newBinding->addPublicAccessingMethodsWithSymbolOnto(symbol, selfFromThis());
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
    if(accessingScope->isProgramEntityScope() && symbol->isLiteralSymbol())
    {
        auto symbolValue = symbol->asString();
        if(symbolValue == "self")
            return selfFromThis();
    }

    auto it = bindings.find(symbol);
    return it != bindings.end() ? it->second.second : nullptr;
}

AnyValuePtr Namespace::lookupExistentLocalSelector(const AnyValuePtr &selector)
{
    return methodDictionary ? methodDictionary->lookupSelector(selector) : AnyValuePtr();
}

AnyValuePtr Namespace::lookupExistentLocalMethodWithSignature(const AnyValuePtr &selector, const TypePtrList &argumentTypes, const TypePtr &resultType, MethodFlags signatureMethodFlags)
{
    if(!methodDictionary)
        return nullptr;

    auto method = methodDictionary->lookupSelector(selector);
    if(!method)
        return nullptr;

    return method->asMethodMatchingSignature(Type::getVoidType(), argumentTypes, resultType);
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


void Namespace::replaceMacroMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector)
{
    if(!macroMethodDictionary)
        macroMethodDictionary = basicMakeObject<MethodDictionary> ();
    macroMethodDictionary->replaceMethodWithSelector(method, selector);
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

SExpression Namespace::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"namespace"}},
        validAnyValue(name)->asSExpression()
    }};
}

SExpression Namespace::asFullDefinitionSExpression() const
{
    SExpressionList childrenSExpr;
    childrenSExpr.elements.reserve(children.size());
    for(auto &child : children)
        childrenSExpr.elements.push_back(child->asFullDefinitionSExpression());

    return SExpressionList{{
        SExpressionIdentifier{{"namespace"}},
        validAnyValue(name)->asSExpression(),
        childrenSExpr
    }};
}

SSAValuePtr Namespace::asSSAValueRequiredInPosition(const ASTSourcePositionPtr &)
{
    if(!ssaNamespace)
    {
        ssaNamespace = basicMakeObject<SSANamespace> ();
        ssaNamespace->setName(getName());
        auto parentSSAValue = getParentProgramEntity()->asProgramEntitySSAValue();
        if(!parentSSAValue->isSSAModule())
        {
            sysmelAssert(parentSSAValue->isSSAProgramEntity());
            parentSSAValue.staticAs<SSAProgramEntity> ()->addChild(ssaNamespace);
        }
    }

    return ssaNamespace;
}

} // End of namespace Environment
} // End of namespace Sysmel