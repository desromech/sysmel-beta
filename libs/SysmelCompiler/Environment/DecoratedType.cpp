#include "Environment/DecoratedType.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/TypeVisitor.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<DecoratedTypeValue> DecoratedTypeValueTypeRegistration;

TypePtr DecoratedType::make(const TypePtr &baseType, TypeDecorationFlags decorations)
{
    if(decorations == TypeDecorationFlags::None)
        return baseType;
    
    auto &cache = RuntimeContext::getActive()->decoratedTypeCache;
    auto it = cache.find({baseType, uint32_t(decorations)});
    if(it != cache.end())
        return it->second;

    auto result = basicMakeObject<DecoratedType> ();
    result->setSupertypeAndImplicitMetaType(DecoratedTypeValue::__staticType__());
    result->baseType = baseType;
    result->decorations = decorations;
    cache.insert({{baseType, uint32_t(decorations)}, result});
    result->addSpecializedInstanceMethods();
    return result;
}

bool DecoratedType::isDecoratedType() const
{
    return true;
}

AnyValuePtr DecoratedType::acceptTypeVisitor(const TypeVisitorPtr &visitor)
{
    return visitor->visitDecoratedType(selfFromThis());
}

AnyValuePtr DecoratedType::lookupLocalSymbolFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope)
{
    return baseType->lookupLocalSymbolFromScope(symbol, accessingScope);
}

ASTNodePtr DecoratedType::analyzeUnboundMessageSendNode(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    return baseType->analyzeMessageSendNode(partiallyAnalyzedNode, semanticAnalyzer);
}

bool DecoratedType::isConstDecoratedType() const
{
    return (decorations & TypeDecorationFlags::Const) != TypeDecorationFlags::None;
}

bool DecoratedType::isVolatileDecoratedType() const
{
    return (decorations & TypeDecorationFlags::Volatile) != TypeDecorationFlags::None;
}

bool DecoratedType::isRestrictDecoratedType() const
{
    return (decorations & TypeDecorationFlags::Restrict) != TypeDecorationFlags::None;
}

bool DecoratedType::isParamsDecoratedType() const
{
    return (decorations & TypeDecorationFlags::Params) != TypeDecorationFlags::None;
}

TypeDecorationFlags DecoratedType::getDecorationFlags() const
{
    return decorations;
}

bool DecoratedType::supportsDynamicCompileTimeMessageSend() const
{
    return baseType->supportsDynamicCompileTimeMessageSend();
}

bool DecoratedType::isNullableType() const
{
    return baseType->isNullableType();
}

bool DecoratedType::isImmutableType()
{
    return baseType->isImmutableType();
}

bool DecoratedType::hasTrivialInitialization()
{
    return baseType->hasTrivialInitialization();
}

bool DecoratedType::hasTrivialInitializationCopyingFrom()
{
    return baseType->hasTrivialInitializationCopyingFrom();
}

bool DecoratedType::hasTrivialInitializationMovingFrom()
{
    return baseType->hasTrivialInitializationMovingFrom();
}

bool DecoratedType::hasTrivialFinalization()
{
    return baseType->hasTrivialFinalization();
}

bool DecoratedType::hasTrivialAssignCopyingFrom()
{
    return baseType->hasTrivialAssignCopyingFrom();
}

bool DecoratedType::hasTrivialAssignMovingFrom()
{
    return baseType->hasTrivialAssignMovingFrom();
}

uint64_t DecoratedType::getMemorySize()
{
    return baseType->getMemorySize();
}

uint64_t DecoratedType::getMemoryAlignment()
{
    return baseType->getMemorySize();
}

TypePtr DecoratedType::withDecorations(TypeDecorationFlags newDecorations)
{
    return make(baseType, decorations | newDecorations);
}

TypePtr DecoratedType::asUndecoratedType()
{
    return baseType;
}

TypePtr DecoratedType::asDecayedType()
{
    return baseType->asDecayedType();
}

TypePtr DecoratedType::asCanonicalArgumentType()
{
    return make(baseType, decorations & TypeDecorationFlags::ArgumentTypeDecorations);
}

TypePtr DecoratedType::asCanonicalResultType()
{
    return make(baseType, decorations & TypeDecorationFlags::ResultTypeDecorations);
}

TypePtr DecoratedType::asInferredTypeForWithModeInEnvironment(const ASTNodePtr &node, TypeInferenceMode mode, bool isMutable, bool concreteLiterals, const ASTAnalysisEnvironmentPtr &environment)
{
    // Remove the decoration from the value type inference.
    if(mode == TypeInferenceMode::Value)
        return baseType->asInferredTypeForWithModeInEnvironment(node, mode, isMutable, concreteLiterals, environment);

    return SuperType::asInferredTypeForWithModeInEnvironment(node, mode, isMutable, concreteLiterals, environment);
}

void DecoratedType::addSpecializedInstanceMethods()
{
}

std::string DecoratedType::getQualifiedName() const
{
    std::ostringstream out;
    out << baseType->getQualifiedName();
    if(isConstDecoratedType())
        out << " const";

    if(isRestrictDecoratedType())
        out << " restrict";

    if(isVolatileDecoratedType())
        out << " volatile";

    if(isParamsDecoratedType())
        out << " params";
    return out.str();
}

SExpression DecoratedType::asSExpression() const
{
    auto result = SExpressionList{{
        SExpressionIdentifier{{"decoratedType"}},
        baseType->asSExpression(),
    }};

    if(isConstDecoratedType())
        result.elements.push_back(SExpressionIdentifier{{"const"}});

    if(isRestrictDecoratedType())
        result.elements.push_back(SExpressionIdentifier{{"restrict"}});

    if(isVolatileDecoratedType())
        result.elements.push_back(SExpressionIdentifier{{"volatile"}});

    return result;
}

DecoratedTypeValuePtr DecoratedType::makeWithValue(const AnyValuePtr &value)
{
    auto pointer = basicMakeObject<DecoratedTypeValue> ();
    pointer->type = selfFromThis();
    pointer->baseValue = value;
    return pointer;
}

bool DecoratedTypeValue::isDecoratedTypeValue() const
{
    return true;
}

AnyValuePtr DecoratedTypeValue::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitDecoratedTypeValue(selfFromThis());
}

} // End of namespace Environment
} // End of namespace Sysmel
