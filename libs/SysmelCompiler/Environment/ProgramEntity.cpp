#include "Environment/ProgramEntity.hpp"
#include "Environment/UnsupportedOperation.hpp"
#include "Environment/Module.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/FieldVariable.hpp"
#include "Environment/MacroInvocationContext.hpp"
#include "Environment/ASTBuilder.hpp"
#include "Environment/ASTIdentifierReferenceNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/StringUtilities.hpp"
#include "Environment/IdentifierLookupScope.hpp"
#include "Environment/Type.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ProgramEntity> programEntityTypeRegistration;

bool ProgramEntity::isProgramEntity() const
{
    return true;
}

ModulePtr ProgramEntity::getDefinitionModule()
{
    return ModulePtr();
}

AnyValuePtr ProgramEntity::getName() const
{
    return nullptr;
}

AnyValuePtr ProgramEntity::getValidName() const
{
    return getName();
}

std::string ProgramEntity::getValidNameString() const
{
    auto nameValue = validAnyValue(getName());
    return nameValue->isUndefined() ? std::string() : nameValue->asString();
}

ProgramEntityPtr ProgramEntity::getMainTemplateInstanceChild() const
{
    return nullptr;
}

std::string ProgramEntity::getQualifiedName() const
{
    std::string result;

    auto parent = getParentProgramEntity();
    if(parent)
    {
        result = parent->getQualifiedName();

        if(validAnyValue(getName())->isAnonymousNameSymbol() && parent->getMainTemplateInstanceChild().get() == this)
            return result;

        result += "::";
    }

    result += getValidNameString();
    return result;
}

std::string ProgramEntity::printString() const
{
    return getQualifiedName();
}

SExpression ProgramEntity::asSExpression() const
{
    return SExpressionSymbol{{getQualifiedName()}};
}

SSAValuePtr ProgramEntity::asProgramEntitySSAValue()
{
    return asSSAValueRequiredInPosition(ASTSourcePosition::empty());
}

TypePtr ProgramEntity::asConstReceiverType()
{
    return Type::getVoidType();
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
    signalNewWithMessage<UnsupportedOperation> (formatString("Cannot bind child program entity {0} with symbol {1} entity inside of {2}.",
        {{binding->printString(), validAnyValue(symbol)->printString(), printString()}}));
}

void ProgramEntity::bindProgramEntityWithVisibility(const ProgramEntityPtr &binding, ProgramEntityVisibility visibility)
{
    bindSymbolWithVisibility(binding->getName(), visibility, binding);
}

void ProgramEntity::enqueuePendingBodyBlockCodeFragment(const DeferredCompileTimeCodeFragmentPtr &codeFragment)
{
    (void)codeFragment;
    signalNewWithMessage<UnsupportedOperation> (formatString("Cannot add an additional definition body to program entity {0}.", {{printString()}}));
}

AnyValuePtr ProgramEntity::lookupExportedSymbolFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope)
{
    (void)symbol;
    (void)accessingScope;
    return nullptr;
}

AnyValuePtr ProgramEntity::lookupLocalSymbolFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope)
{
    if(accessingScope->isProgramEntityScope() && symbol->isLiteralSymbol())
    {
        auto symbolValue = symbol->asString();
        if(symbolValue == "self")
            return selfFromThis();
    }

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

AnyValuePtr ProgramEntity::lookupExistentLocalSelector(const AnyValuePtr &selector)
{
    (void)selector;
    return nullptr;
}

AnyValuePtr ProgramEntity::lookupLocalSelector(const AnyValuePtr &selector)
{
    return lookupExistentLocalSelector(selector);
}

AnyValuePtr ProgramEntity::lookupLocalMacroFallbackSelector(const AnyValuePtr &selector)
{
    (void)selector;
    return nullptr;
}

SpecificMethodPtr ProgramEntity::lookupParentOverridenMethod(const AnyValuePtr &, bool, const TypePtrList &)
{
    return nullptr;
}

void ProgramEntity::addVirtualMethod(const SpecificMethodPtr &method)
{
    signalNewWithMessage<UnsupportedOperation> (formatString("Cannot define virtual method {0} in {1}.", {{method->printString(), printString()}}));
}

void ProgramEntity::addFieldVariableWithVisibility(const FieldVariablePtr &field, ProgramEntityVisibility visibility)
{
    (void)field;
    (void)visibility;
    signalNewWithMessage<UnsupportedOperation> (formatString("Cannot define field variable {0} in {1}.", {{field->printString(), printString()}}));
}

void ProgramEntity::addFieldVariable(const FieldVariablePtr &field)
{
    addFieldVariableWithVisibility(field, ProgramEntityVisibility::Protected);
}

void ProgramEntity::addFieldVariables(const FieldVariablePtrList &fields)
{
    for(auto &field : fields)
        addFieldVariable(field);
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

bool ProgramEntity::canHaveUserDefinedFields() const
{
    return false;
}

bool ProgramEntity::canHaveVirtualMethods() const
{
    return false;
}

void ProgramEntity::addPublicAccessingMethodsWithSymbolOnto(const AnyValuePtr &symbol, const ProgramEntityPtr &programEntity)
{
    auto self = selfFromThis();
    programEntity->addMacroMethodCategories({{"accessing", {
        makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> (symbol, [=](const MacroInvocationContextPtr &macroContext) {
            return macroContext->astBuilder->identifierWithBinding(symbol, self);
        }, MethodFlags::Macro),
    }}});
}

void ProgramEntity::addPublicInstanceAccessingMethodsWithSymbolOnto(const AnyValuePtr &symbol, const TypePtr &type)
{
    (void)symbol;
    (void)type;
}

ProgramEntityPtr ProgramEntity::getParentProgramEntity() const
{
    return parentProgramEntity.lock();
}

void ProgramEntity::setParentProgramEntity(const ProgramEntityPtr &newParent)
{
    parentProgramEntity = newParent;
}

} // End of namespace Environment
} // End of namespace Sysmel
