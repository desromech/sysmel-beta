#include "Environment/ClosureType.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/TypeVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ClosureTypeValue> functionalTypeRegistration;

ClosureTypePtr ClosureType::make(const TypePtr &resultType, const TypePtrList &arguments)
{
    auto canonicalResultType = resultType->asUndecoratedType();
    auto canonicalArgumentTypes = arguments;
    for(auto &el : canonicalArgumentTypes)
        el = el->asUndecoratedType();

    auto &cache = RuntimeContext::getActive()->closureTypeCache;
    auto it = cache.find({canonicalResultType, canonicalArgumentTypes});
    if(it != cache.end())
        return it->second;

    auto result = basicMakeObject<ClosureType> ();
    result->arguments = canonicalArgumentTypes;
    result->result = canonicalResultType;
    result->setSupertypeAndImplicitMetaType(ClosureTypeValue::__staticType__());
    cache.insert({{canonicalResultType, canonicalArgumentTypes}, result});
    return result;
}

ClosureTypePtr getOrCreateClosureType(const TypePtr &resultType, const TypePtrList &arguments)
{
    return ClosureType::make(resultType, arguments);
}

bool ClosureType::isClosureType() const
{
    return true;
}

AnyValuePtr ClosureType::acceptTypeVisitor(const TypeVisitorPtr &visitor)
{
    return visitor->visitClosureType(selfFromThis());
}

std::string ClosureType::getQualifiedName() const
{
    return "(" + SuperType::getQualifiedName() + ") closure";
}

SExpression ClosureType::asSExpression() const
{
    SExpressionList argumentsSExpr;
    argumentsSExpr.elements.reserve(arguments.size());
    for(auto &arg : arguments)
        argumentsSExpr.elements.push_back(arg->asSExpression());

    return SExpressionList{{
        SExpressionIdentifier{{"closureType"}},
        argumentsSExpr,
        result->asSExpression()
    }};
}

FunctionalTypeValuePtr ClosureType::makeValueWithEnvironmentAndImplementation(const AnyValuePtr &environment, const AnyValuePtr &implementation)
{
    auto resultValue = basicMakeObject<ClosureTypeValue> ();
    resultValue->type = selfFromThis();
    resultValue->functionalImplementation = implementation;
    resultValue->environment = environment;
    return resultValue;
}

bool ClosureTypeValue::isClosureTypeValue() const
{
    return true;
}

TypePtr ClosureTypeValue::getType() const
{
    return type;
}

AnyValuePtr ClosureTypeValue::applyWithArguments(const std::vector<AnyValuePtr> &arguments)
{
    return functionalImplementation->applyInClosureWithArguments(environment, arguments);
}

} // End of namespace Environment
} // End of namespace Sysmel