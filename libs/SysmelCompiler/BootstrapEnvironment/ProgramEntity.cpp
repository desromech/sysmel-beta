#include "sysmel/BootstrapEnvironment/ProgramEntity.hpp"
#include "sysmel/BootstrapEnvironment/UnsupportedOperation.hpp"
#include "sysmel/BootstrapEnvironment/Module.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/StringUtilities.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ProgramEntity> programEntityTypeRegistration;

bool ProgramEntity::isProgramEntity() const
{
    return true;
}

ModulePtr ProgramEntity::getDefinitionModule() const
{
    return ModulePtr();
}

AnyValuePtr ProgramEntity::getName() const
{
    return nullptr;
}

TypePtr ProgramEntity::asReceiverType()
{
    return Type::getVoidType();
}

void ProgramEntity::recordChildProgramEntityDefinition(const ProgramEntityPtr &newChild)
{
    signalNewWithMessage<UnsupportedOperation> (formatString("Cannot record child program {0} entity inside of {1}.",
        {{newChild->printString(), printString()}}));
}

void ProgramEntity::bindSymbolWithVisibility(const AnyValuePtr &symbol, ProgramEntityVisibility visibility, const ProgramEntityPtr &binding)
{
    (void)visibility;
    signalNewWithMessage<UnsupportedOperation> (formatString("Cannot bind child program {0} with symbol {1} entity inside of {2}.",
        {{binding->printString(), validAnyValue(symbol)->printString(), printString()}}));
}

void ProgramEntity::bindProgramEntityWithVisibility(const ProgramEntityPtr &binding, ProgramEntityVisibility visibility)
{
    bindSymbolWithVisibility(binding->getName(), visibility, binding);
}

AnyValuePtr ProgramEntity::lookupExportedSymbolFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope)
{
    (void)symbol;
    (void)accessingScope;
    return nullptr;
}

AnyValuePtr ProgramEntity::lookupLocalSymbolFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope)
{
    (void)symbol;
    (void)accessingScope;
    return nullptr;
}

AnyValuePtr ProgramEntity::lookupLocalMacroSelector(const AnyValuePtr &selector)
{
    (void)selector;
    return nullptr;
}

AnyValuePtr ProgramEntity::lookupSelectorInReceiverNodeWithExpansionLevel(const AnyValuePtr &selector, const ASTNodePtr &receiverNode, MessageSendExpansionLevel expansionLevel)
{
    (void)receiverNode;

    switch(expansionLevel)
    {
    case MessageSendExpansionLevel::UnexpandedMacros:
        return lookupMacroSelector(selector);
    case MessageSendExpansionLevel::ExpandedMacros:
        return lookupSelector(selector);
    case MessageSendExpansionLevel::FallbackMacros:
        return lookupMacroFallbackSelector(selector);
    default:
        return nullptr;
    }
}

AnyValuePtr ProgramEntity::lookupMacroSelector(const AnyValuePtr &selector)
{
    return lookupLocalMacroSelector(selector);
}

AnyValuePtr ProgramEntity::lookupMacroFallbackSelector(const AnyValuePtr &selector)
{
    return lookupLocalMacroFallbackSelector(selector);
}

AnyValuePtr ProgramEntity::lookupSelector(const AnyValuePtr &selector)
{
    return lookupLocalSelector(selector);
}

AnyValuePtr ProgramEntity::lookupLocalSelector(const AnyValuePtr &selector)
{
    (void)selector;
    return nullptr;
}

AnyValuePtr ProgramEntity::lookupLocalMacroFallbackSelector(const AnyValuePtr &selector)
{
    (void)selector;
    return nullptr;
}

void ProgramEntity::addMacroMethodWithSelector(const AnyValuePtr &selector, const AnyValuePtr &method)
{
    (void)selector;
    (void)method;
    signalNewWithMessage<UnsupportedOperation> (formatString("Cannot define macro method {0} in {1}.", {{method->printString(), printString()}}));
}

void ProgramEntity::addMacroMethodCategories(const MethodCategories &categories)
{
    for(auto &[category, methods] : categories)
    {
        for(auto &[selector, method] : methods)
            addMacroMethodWithSelector(method, selector);
    }
}

void ProgramEntity::addMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector)
{
    (void)selector;
    (void)method;
    signalNewWithMessage<UnsupportedOperation> (formatString("Cannot define method {0} in {1}.", {{method->printString(), printString()}}));
}

void ProgramEntity::addMethodCategories(const MethodCategories &categories)
{
    for(auto &[category, methods] : categories)
    {
        for(auto &[selector, method] : methods)
            addMethodWithSelector(method, selector);
    }
}

void ProgramEntity::addMacroFallbackMethodWithSelector(const AnyValuePtr &selector, const AnyValuePtr &method)
{
    (void)selector;
    (void)method;
    signalNewWithMessage<UnsupportedOperation> (formatString("Cannot define macro fallback method {0} in {1}.", {{method->printString(), printString()}}));
}

void ProgramEntity::addMacroFallbackMethodCategories(const MethodCategories &categories)
{
    for(auto &[category, methods] : categories)
    {
        for(auto &[selector, method] : methods)
            addMacroFallbackMethodWithSelector(method, selector);
    }
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius
