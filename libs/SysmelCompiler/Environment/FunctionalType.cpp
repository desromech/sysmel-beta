#include "Environment/FunctionalType.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/SubclassResponsibility.hpp"
#include "Environment/PrimitiveBooleanType.hpp"
#include "Environment/Undefined.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<FunctionalTypeValue> functionalTypeRegistration;

bool FunctionalType::isFunctionalType() const
{
    return true;
}

size_t FunctionalType::getArgumentCount() const
{
    return arguments.size();
}

const TypePtr &FunctionalType::getArgument(size_t index) const
{
    sysmelAssert(index < arguments.size());
    return arguments[index];
}

const TypePtr &FunctionalType::getResultType() const
{
    return result;
}

TypePtr FunctionalType::getReceiverType() const
{
    return Type::getVoidType();
}

std::string FunctionalType::getQualifiedName() const
{
    std::ostringstream out;
    out << '(';
    if(arguments.empty())
    {
        out << "Void";
    }
    else
    {
        for(size_t i = 0; i < arguments.size(); ++i)
        {
            if(i > 0)
                out << ", ";
            out << arguments[i]->getQualifiedName();
        }
    }
    out << ") => ";
    out << result->getQualifiedName();

    return out.str();
}

ASTNodePtr FunctionalType::analyzeCallNode(const ASTCallNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    return semanticAnalyzer->analyzeCallNodeWithFunctionalType(node, selfFromThis());
}

FunctionalTypeValuePtr FunctionalType::makeValueWithImplementation(const AnyValuePtr &)
{
    SysmelSelfSubclassResponsibility();
}

FunctionalTypeValuePtr FunctionalType::makeValueWithEnvironmentAndImplementation(const AnyValuePtr &, const AnyValuePtr &)
{
    SysmelSelfSubclassResponsibility();
}

bool FunctionalType::hasAutoResultType() const
{
    return result->isAutoType();
}

bool FunctionalType::supportsDynamicCompileTimeMessageSend() const
{
    return false;
}

bool FunctionalType::isNullableType() const
{
    return false;
}

bool FunctionalType::isImmutableType()
{
    return true;
}

bool FunctionalType::hasTrivialInitialization()
{
    return true;
}

bool FunctionalType::hasTrivialInitializationCopyingFrom()
{
    return true;
}

bool FunctionalType::hasTrivialInitializationMovingFrom()
{
    return true;
}

uint64_t FunctionalType::getMemorySize()
{
    return RuntimeContext::getActive()->getTargetDescription().pointerSize;
}

uint64_t FunctionalType::getMemoryAlignment()
{
    return RuntimeContext::getActive()->getTargetDescription().pointerAlignment;
}

bool FunctionalType::hasTrivialFinalization()
{
    return true;
}

bool FunctionalType::hasTrivialAssignCopyingFrom()
{
    return true;
}

bool FunctionalType::hasTrivialAssignMovingFrom()
{
    return true;
}

void FunctionalType::addSpecializedInstanceMethods()
{
    // nil -> functional.
    addConstructor(makeIntrinsicConstructorWithSignature<FunctionalTypeValuePtr (FunctionalTypePtr, UndefinedPtr)> ("pointer.null", getType(), selfFromThis(), {Undefined::__staticType__()}, [=](const FunctionalTypePtr &, const UndefinedPtr &) -> FunctionalTypeValuePtr {
        return staticObjectCast<FunctionalTypeValue> (makeValueWithEnvironmentAndImplementation(nullptr, nullptr));
    }, MethodFlags::Pure));

    auto booleanType = Boolean8::__staticType__();
    addMethodCategories(MethodCategories{
        {"comparisons", {
            makeIntrinsicMethodBindingWithSignature<Boolean8Ptr (FunctionalTypeValuePtr, FunctionalTypeValuePtr)> ("pointer.equals", "=", selfFromThis(), booleanType, {selfFromThis()}, [=](const FunctionalTypeValuePtr &a, const FunctionalTypeValuePtr &b) -> Boolean8Ptr {
                return Boolean8::make(a == b);
            }, MethodFlags::NotInCompileTime),
            makeIntrinsicMethodBindingWithSignature<Boolean8Ptr (FunctionalTypeValuePtr, FunctionalTypeValuePtr)> ("pointer.equals", "==", selfFromThis(), booleanType, {selfFromThis()}, [=](const FunctionalTypeValuePtr &a, const FunctionalTypeValuePtr &b) -> Boolean8Ptr {
                return Boolean8::make(a == b);
            }, MethodFlags::NotInCompileTime),

            makeIntrinsicMethodBindingWithSignature<Boolean8Ptr (FunctionalTypeValuePtr, FunctionalTypeValuePtr)> ("pointer.not-equals", "~=", selfFromThis(), booleanType, {selfFromThis()}, [=](const FunctionalTypeValuePtr &a, const FunctionalTypeValuePtr &b) -> Boolean8Ptr {
                return Boolean8::make(a != b);
            }, MethodFlags::NotInCompileTime),
            makeIntrinsicMethodBindingWithSignature<Boolean8Ptr (FunctionalTypeValuePtr, FunctionalTypeValuePtr)> ("pointer.not-equals", "~~", selfFromThis(), booleanType, {selfFromThis()}, [=](const FunctionalTypeValuePtr &a, const FunctionalTypeValuePtr &b) -> Boolean8Ptr {
                return Boolean8::make(a != b);
            }, MethodFlags::NotInCompileTime),
        }},
    });
}

FunctionTypePtr FunctionalType::copyWithResultType(const TypePtr &newResultType)
{
    (void)newResultType;
    SysmelSelfSubclassResponsibility();
}

bool FunctionalType::matchesDefinitionSignature(bool hasReceiver, bool hasConstReceiver, const TypePtrList &testArgumentTypes, const TypePtr &testResultType)
{
    auto receiverType = getReceiverType();
    auto matchesReceiver = (!hasReceiver && receiverType->isVoidType()) ||
        (hasReceiver && !receiverType->isVoidType() && receiverType->isConstOrConstReferenceType() == hasConstReceiver);

    return matchesReceiver && arguments == testArgumentTypes && (!testResultType || result == testResultType);
}

bool FunctionalType::matchesSignature(const TypePtr &testReceiverType, const TypePtrList &testArgumentTypes, const TypePtr &testResultType) const
{
    return getReceiverType() == testReceiverType && arguments == testArgumentTypes && (result == testResultType || !testResultType);
}

TypePtr FunctionalType::getParamsType() const
{
    if(!arguments.empty() && arguments.back()->isParamsDecoratedType())
        return arguments.back();
    return nullptr;
}

bool FunctionalType::hasCVarArgs() const
{
    auto paramsType = getParamsType();
    return paramsType && paramsType->isCVarArgsType();
}

bool FunctionalTypeValue::isFunctionalTypeValue() const
{
    return true;
}

SExpression FunctionalTypeValue::asSExpression() const
{
    return functionalImplementation->asSExpression();
}

} // End of namespace Environment
} // End of namespace Sysmel