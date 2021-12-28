#include "Environment/MethodType.hpp"
#include "Environment/TypeVisitor.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"


namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<MethodTypeValue> functionalTypeRegistration;

MethodTypePtr MethodType::make(const TypePtr &receiverType, const TypePtr &resultType, const TypePtrList &arguments)
{
    auto canonicalReceiverType = receiverType->asUndecoratedType();
    auto canonicalResultType = resultType->asUndecoratedType();
    auto canonicalArgumentTypes = arguments;
    for(auto &el : canonicalArgumentTypes)
        el = el->asUndecoratedType();

    auto &cache = RuntimeContext::getActive()->methodTypeCache;
    auto it = cache.find({canonicalReceiverType, canonicalResultType, canonicalArgumentTypes});
    if(it != cache.end())
        return it->second;

    auto result = basicMakeObject<MethodType> ();
    result->receiverType = canonicalReceiverType;
    result->arguments = canonicalArgumentTypes;
    result->result = canonicalResultType;
    result->setSupertypeAndImplicitMetaType(MethodTypeValue::__staticType__());
    cache.insert({{canonicalReceiverType, canonicalResultType, canonicalArgumentTypes}, result});
    return result;
}

TypePtr MethodType::getReceiverType() const
{
    return receiverType;
}

FunctionalTypeValuePtr MethodType::makeValueWithImplementation(const AnyValuePtr &implementation)
{
    auto resultValue = basicMakeObject<MethodTypeValue> ();
    resultValue->type = selfFromThis();
    resultValue->functionalImplementation = implementation;
    return resultValue;
}

MethodTypePtr getOrCreateMethodType(const TypePtr &receiverType, const TypePtr &resultType, const TypePtrList &arguments)
{
    return MethodType::make(receiverType, resultType, arguments);
}

bool MethodType::isMethodType() const
{
    return true;
}

AnyValuePtr MethodType::acceptTypeVisitor(const TypeVisitorPtr &visitor)
{
    return visitor->visitMethodType(selfFromThis());
}

std::string MethodType::printString() const
{
    return "((" + SuperType::printString() + ") methodWithReceiver: " + receiverType->printString() + ")";
}

SExpression MethodType::asSExpression() const
{
    SExpressionList argumentsSExpr;
    argumentsSExpr.elements.reserve(arguments.size());
    for(auto &arg : arguments)
        argumentsSExpr.elements.push_back(arg->asSExpression());

    return SExpressionList{{
        SExpressionIdentifier{{"methodType"}},
        receiverType->asSExpression(),
        argumentsSExpr,
        result->asSExpression()
    }};
}

bool MethodTypeValue::isMethodTypeValue() const
{
    return true;
}

TypePtr MethodTypeValue::getType() const
{
    return type;
}

} // End of namespace Environment
} // End of namespace Sysmel