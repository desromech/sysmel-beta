#include "Environment/DecoratedType.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
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

bool DecoratedType::hasTrivialFinalization()
{
    return baseType->hasTrivialFinalization();
}

bool DecoratedType::hasTrivialCopyingFrom()
{
    return baseType->hasTrivialCopyingFrom();
}

bool DecoratedType::hasTrivialMovingFrom()
{
    return baseType->hasTrivialMovingFrom();
}

TypePtr DecoratedType::withDecorations(TypeDecorationFlags newDecorations)
{
    return make(baseType, decorations | newDecorations);
}

TypePtr DecoratedType::asUndecoratedType()
{
    return baseType;
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

std::string DecoratedType::printString() const
{
    std::ostringstream out;
    out << baseType->printString();
    if(isConstDecoratedType())
        out << " const";

    if(isRestrictDecoratedType())
        out << " restrict";

    if(isVolatileDecoratedType())
        out << " volatile";
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

} // End of namespace Environment
} // End of namespace Sysmel