#include "Environment/FunctionType.hpp"
#include "Environment/ClosureType.hpp"
#include "Environment/MethodType.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/TypeVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<FunctionTypeValue> functionalTypeRegistration;

FunctionTypePtr FunctionType::make(const TypePtr &resultType, const TypePtrList &arguments)
{
    auto canonicalResultType = resultType->asCanonicalResultType();
    auto canonicalArgumentTypes = arguments;
    for(auto &el : canonicalArgumentTypes)
        el = el->asCanonicalArgumentType();

    auto &cache = RuntimeContext::getActive()->functionTypeCache;
    auto it = cache.find({canonicalResultType, canonicalArgumentTypes});
    if(it != cache.end())
        return it->second;

    auto result = basicMakeObject<FunctionType> ();
    result->setSupertypeAndImplicitMetaType(FunctionTypeValue::__staticType__());
    result->arguments = canonicalArgumentTypes;
    result->result = canonicalResultType;
    cache.insert({{canonicalResultType, canonicalArgumentTypes}, result});
    result->addSpecializedMethods();
    return result;
}

FunctionTypePtr FunctionType::copyWithResultType(const TypePtr &newResultType)
{
    return make(newResultType, arguments);
}

FunctionTypePtr getOrCreateFunctionType(const TypePtr &resultType, const TypePtrList &arguments)
{
    return FunctionType::make(resultType, arguments);
}

bool FunctionType::isFunctionType() const
{
    return true;
}

AnyValuePtr FunctionType::acceptTypeVisitor(const TypeVisitorPtr &visitor)
{
    return visitor->visitFunctionType(selfFromThis());
}

FunctionalTypeValuePtr FunctionType::makeValueWithImplementation(const AnyValuePtr &implementation)
{
    auto resultValue = basicMakeObject<FunctionTypeValue> ();
    resultValue->type = selfFromThis();
    resultValue->functionalImplementation = implementation;
    return resultValue;
}

SExpression FunctionType::asSExpression() const
{
    SExpressionList argumentsSExpr;
    argumentsSExpr.elements.reserve(arguments.size());
    for(auto &arg : arguments)
        argumentsSExpr.elements.push_back(arg->asSExpression());

    return SExpressionList{{
        SExpressionIdentifier{{"functionType"}},
        argumentsSExpr,
        result->asSExpression()
    }};
}

void FunctionType::addSpecializedMethods()
{
    getType()->addMethodCategories(MethodCategories{
        {"type composition", {
            makeMethodBinding<TypePtr (TypePtr)> ("closure", +[](const TypePtr &self) {
                auto selfFunctionType = staticObjectCast<FunctionType> (self);
                return ClosureType::make(selfFunctionType->result, selfFunctionType->arguments);
            }, MethodFlags::Pure),
            makeMethodBinding<TypePtr (TypePtr, TypePtr)> ("methodWithReceiver:", +[](const TypePtr &self, const TypePtr &receiverType) {
                auto selfFunctionType = staticObjectCast<FunctionType> (self);
                return MethodType::make(receiverType, selfFunctionType->result, selfFunctionType->arguments);
            }, MethodFlags::Pure),
        }}
    });
}

bool FunctionTypeValue::isFunctionTypeValue() const
{
    return true;
}

TypePtr FunctionTypeValue::getType() const
{
    return type;
}

AnyValuePtr FunctionTypeValue::applyWithArguments(const std::vector<AnyValuePtr> &arguments)
{
    return functionalImplementation->applyWithArguments(arguments);
}

SSAValuePtr FunctionTypeValue::asSSAValueRequiredInPosition(const ASTSourcePositionPtr &requiredSourcePosition)
{
    return functionalImplementation->asSSAValueRequiredInPosition(requiredSourcePosition);
}

} // End of namespace Environment
} // End of namespace Sysmel