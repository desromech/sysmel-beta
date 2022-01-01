#include "Environment/PrimitiveVectorType.hpp"
#include "Environment/PrimitiveBooleanType.hpp"
#include "Environment/PrimitiveIntegerType.hpp"
#include "Environment/PrimitiveFloatType.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/TypeVisitor.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/Error.hpp"
#include "Environment/ASTNode.hpp"
#include "Environment/ASTMakeVectorNode.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/Utilities.hpp"
#include <algorithm>
#include <sstream>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<PrimitiveVectorTypeValue> PrimitiveVectorTypeValueTypeRegistration;

TypePtr PrimitiveVectorType::make(const TypePtr &elementType, uint32_t elements)
{
    auto undecoratedElementType = elementType->asUndecoratedType();
    if(elements <= 1)
        return elementType;
    
    auto &cache = RuntimeContext::getActive()->primitiveVectorTypeCache;
    auto it = cache.find({undecoratedElementType, elements});
    if(it != cache.end())
        return it->second;

    auto result = basicMakeObject<PrimitiveVectorType> ();
    result->setSupertypeAndImplicitMetaType(PrimitiveVectorTypeValue::__staticType__());
    result->elementType = undecoratedElementType;
    result->elements = elements;
    cache.insert({{undecoratedElementType, elements}, result});
    result->addSpecializedInstanceMethods();
    return result;
}

AnyValuePtr PrimitiveVectorType::acceptTypeVisitor(const TypeVisitorPtr &visitor)
{
    return visitor->visitPrimitiveVectorType(selfFromThis());
}

bool PrimitiveVectorType::supportsDynamicCompileTimeMessageSend() const
{
    return false;
}

bool PrimitiveVectorType::isPrimitiveVectorType() const
{
    return true;
}

bool PrimitiveVectorType::isNullableType() const
{
    return false;
}

bool PrimitiveVectorType::hasTrivialInitialization()
{
    return true;
}

bool PrimitiveVectorType::hasTrivialInitializationCopyingFrom()
{
    return true;
}

bool PrimitiveVectorType::hasTrivialInitializationMovingFrom()
{
    return true;
}

bool PrimitiveVectorType::hasTrivialFinalization()
{
    return true;
}

bool PrimitiveVectorType::hasTrivialAssignCopyingFrom()
{
    return true;
}

bool PrimitiveVectorType::hasTrivialAssignMovingFrom()
{
    return true;
}

uint64_t PrimitiveVectorType::getMemorySize()
{
    return elementType->getAlignedMemorySize() * elements;
}

uint64_t PrimitiveVectorType::getMemoryAlignment()
{
    auto alignmentFactor = asHighPowerOfTwo(elements);
    return elementType->getMemoryAlignment()*alignmentFactor;
}

std::string PrimitiveVectorType::printString() const
{
    if(name && !name->isAnonymousNameSymbol())
        return SuperType::printString();

    std::ostringstream out;
    out << '(';
    out << elementType->printString();
    out << " primitiveVectorWithElements: " << elements << ')';
    return out.str();
}

SExpression PrimitiveVectorType::asSExpression() const
{
    if(name && !name->isAnonymousNameSymbol())
        return SuperType::asSExpression();

    return SExpressionList{{
        SExpressionIdentifier{{"primitiveVectorType"}},
        elementType->asSExpression(),
        LargeInteger{elements}
    }};
}

PrimitiveVectorTypeValuePtr PrimitiveVectorType::withValues(const AnyValuePtrList &valueElements)
{
    auto vector = basicMakeObject<PrimitiveVectorTypeValue> ();
    vector->type = selfFromThis();
    vector->elements = valueElements;
    return vector;
}

PrimitiveVectorTypeValuePtr PrimitiveVectorType::withAll(const AnyValuePtr &element)
{
    auto vector = basicMakeObject<PrimitiveVectorTypeValue> ();
    vector->type = selfFromThis();
    vector->elements.resize(elements, element);
    return vector;
}

AnyValuePtr PrimitiveVectorType::basicNewValue()
{
    return withAll(elementType->basicNewValue());
}
ASTNodePtr PrimitiveVectorType::analyzeFallbackValueConstructionWithArguments(const ASTNodePtr &node, const ASTNodePtrList &arguments, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    TypePtrList decayedTypes;
    decayedTypes.reserve(arguments.size());
    uint32_t resultRank = 0;
    auto constructionArguments = arguments;
    ASTNodePtr errorNode;
    
    for(auto &arg : constructionArguments)
    {
        arg = semanticAnalyzer->analyzeNodeIfNeededWithAutoType(arg);
        
        auto decayedArgumentType = arg->analyzedType->asDecayedType();
        if(decayedArgumentType->isPrimitiveVectorType())
        {
            auto vectorArgumentSize = decayedArgumentType.staticAs<PrimitiveVectorType> ()->elements;
            auto expectedVectorArgumentType = PrimitiveVectorType::make(elementType, vectorArgumentSize);
            arg = semanticAnalyzer->analyzeNodeIfNeededWithExpectedType(arg, expectedVectorArgumentType);
            resultRank += vectorArgumentSize;
        }
        else
        {
            arg = semanticAnalyzer->analyzeNodeIfNeededWithExpectedType(arg, elementType);
            ++resultRank;
        }

        if(arg->isASTErrorNode())
            errorNode = arg;
    }

    if(resultRank != elements)
        return semanticAnalyzer->recordSemanticErrorInNode(node, "Invalid number of components provided for making the requested primitive vector.");
    if(errorNode)
        return errorNode;
    
    auto result = basicMakeObject<ASTMakeVectorNode>();
    result->sourcePosition = node->sourcePosition;
    result->elements = constructionArguments;
    result->analyzedType = selfFromThis();
    return result;
}

void PrimitiveVectorType::addSpecializedInstanceMethods()
{
    auto accessElement = [](const PrimitiveVectorTypeValuePtr &self, const AnyValuePtr &index) {
        auto indexValue = index->unwrapAsInt64();
        if(indexValue < 0 || size_t(indexValue) > self->elements.size())
            signalNewWithMessage<Error> ("Vector element index is out of range.");

        return self->elements[indexValue];
    };

    // Exposed constructors.
    getType()->addMethodCategories(MethodCategories{
        {"constructors", {
            makeIntrinsicMethodBindingWithSignature<PrimitiveVectorTypeValuePtr (PrimitiveVectorTypePtr, AnyValuePtr)> ("vector.make.with-all", "withAll:", getType(), selfFromThis(), {elementType}, [](const PrimitiveVectorTypePtr &vectorType, const AnyValuePtr &element){
                return vectorType->withAll(element);
            }, MethodFlags::Pure | MethodFlags::Constructor),
        }},
    });

    // Accessing methods.
    addMethodCategories(MethodCategories{
        {"accessing", {
            // []
            makeIntrinsicMethodBindingWithSignature<AnyValuePtr (PrimitiveVectorTypeValuePtr, AnyValuePtr)> ("vector.element", "[]", selfFromThis(), elementType, {Type::getIntPointerType()}, accessElement, MethodFlags::Pure),

            // x
            makeIntrinsicMethodBindingWithSignature<AnyValuePtr (PrimitiveVectorTypeValuePtr)> ("vector.element.first", "x", selfFromThis(), elementType, {}, [](const PrimitiveVectorTypeValuePtr &self){
                return self->elements[0];
            }, MethodFlags::Pure),
        }},
    });

    if(elements >= 2)
    {
        addMethodCategories(MethodCategories{
            {"accessing", {
                // y
                makeIntrinsicMethodBindingWithSignature<AnyValuePtr (PrimitiveVectorTypeValuePtr)> ("vector.element.second", "y", selfFromThis(), elementType, {}, [](const PrimitiveVectorTypeValuePtr &self){
                    return self->elements[1];
                }, MethodFlags::Pure),
            }},
        });
    }

    if(elements >= 3)
    {
        addMethodCategories(MethodCategories{
            {"accessing", {
                // z
                makeIntrinsicMethodBindingWithSignature<AnyValuePtr (PrimitiveVectorTypeValuePtr)> ("vector.element.third", "z", selfFromThis(), elementType, {}, [](const PrimitiveVectorTypeValuePtr &self){
                    return self->elements[2];
                }, MethodFlags::Pure),
            }},
        });
    }

    if(elements >= 4)
    {
        addMethodCategories(MethodCategories{
            {"accessing", {
                // w
                makeIntrinsicMethodBindingWithSignature<AnyValuePtr (PrimitiveVectorTypeValuePtr)> ("vector.element.fourth", "w", selfFromThis(), elementType, {}, [](const PrimitiveVectorTypeValuePtr &self){
                    return self->elements[3];
                }, MethodFlags::Pure),
            }},
        });
    }

    // Comparisons.
    auto equals = [](const PrimitiveVectorTypeValuePtr &self, const PrimitiveVectorTypeValuePtr &other){
        auto result = true;
        for(uint32_t i = 0; i < self->elements.size(); ++i)
        {
            if(!self->elements[i]->perform<bool> ("=", other->elements[i]))
            {
                result = false;
                break;
            }
        }
        return Boolean8::make(result);
    };
    auto notEquals = [](const PrimitiveVectorTypeValuePtr &self, const PrimitiveVectorTypeValuePtr &other){
        auto result = false;
        for(uint32_t i = 0; i < self->elements.size(); ++i)
        {
            if(self->elements[i]->perform<bool> ("~=", other->elements[i]))
            {
                result = true;
                break;
            }
        }
        return Boolean8::make(result);
    };

    auto isFloatingPoint = elementType->isSubtypeOf(PrimitiveFloatType::__staticType__());
    auto isBoolean = elementType == Boolean8::__staticType__();
    auto isSigned = elementType == Int8::__staticType__() ||
                elementType == Int16::__staticType__() ||
                elementType == Int32::__staticType__() ||
                elementType == Int64::__staticType__();

    std::string equalitySuffix = isFloatingPoint ? ".float" : ".integer";
    std::string comparisonSuffix;
    if(isBoolean)
        comparisonSuffix = ".boolean";
    else if(isFloatingPoint)
        comparisonSuffix = ".float";
    else
        comparisonSuffix = isSigned ? ".integer.signed" : ".integer.unsigned";

    addMethodCategories(MethodCategories{
        {"comparisons", {
            // =
            makeIntrinsicMethodBindingWithSignature<AnyValuePtr (PrimitiveVectorTypeValuePtr, PrimitiveVectorTypeValuePtr)> ("vector.equals" + equalitySuffix, "=", selfFromThis(), Boolean8::__staticType__(), {selfFromThis()}, equals, MethodFlags::Pure),
            makeIntrinsicMethodBindingWithSignature<AnyValuePtr (PrimitiveVectorTypeValuePtr, PrimitiveVectorTypeValuePtr)> ("vector.equals" + equalitySuffix, "==", selfFromThis(), Boolean8::__staticType__(), {selfFromThis()}, equals, MethodFlags::Pure),
            // ~=
            makeIntrinsicMethodBindingWithSignature<AnyValuePtr (PrimitiveVectorTypeValuePtr, PrimitiveVectorTypeValuePtr)> ("vector.not-equals" + equalitySuffix, "~=", selfFromThis(), Boolean8::__staticType__(), {selfFromThis()}, notEquals, MethodFlags::Pure),
            makeIntrinsicMethodBindingWithSignature<AnyValuePtr (PrimitiveVectorTypeValuePtr, PrimitiveVectorTypeValuePtr)> ("vector.not-equals" + equalitySuffix, "~~", selfFromThis(), Boolean8::__staticType__(), {selfFromThis()}, notEquals, MethodFlags::Pure),
        }}
    });

    // Reduction.
    if(isBoolean)
    {
        addMethodCategories(MethodCategories{
            {"reductions", {
                makeIntrinsicMethodBindingWithSignature<AnyValuePtr (PrimitiveVectorTypeValuePtr)> ("vector.any", "isAnySet", selfFromThis(), elementType, {}, [](const PrimitiveVectorTypeValuePtr &self){
                    return self->reduce("|");
                }, MethodFlags::Pure),
                makeIntrinsicMethodBindingWithSignature<AnyValuePtr (PrimitiveVectorTypeValuePtr)> ("vector.all", "isAllSet", selfFromThis(), elementType, {}, [](const PrimitiveVectorTypeValuePtr &self){
                    return self->reduce("&");
                }, MethodFlags::Pure),
            }}
        });
    }
    else
    {
        if(!isFloatingPoint)
        {
            addMethodCategories(MethodCategories{
                {"reductions", {
                    makeIntrinsicMethodBindingWithSignature<AnyValuePtr (PrimitiveVectorTypeValuePtr)> ("vector.reduce.or", "reduceWithOr", selfFromThis(), elementType, {}, [](const PrimitiveVectorTypeValuePtr &self){
                        return self->reduce("|");
                    }, MethodFlags::Pure),
                    makeIntrinsicMethodBindingWithSignature<AnyValuePtr (PrimitiveVectorTypeValuePtr)> ("vector.reduce.and" + equalitySuffix, "reduceWithAnd", selfFromThis(), elementType, {}, [](const PrimitiveVectorTypeValuePtr &self){
                        return self->reduce("&");
                    }, MethodFlags::Pure),
                    makeIntrinsicMethodBindingWithSignature<AnyValuePtr (PrimitiveVectorTypeValuePtr)> ("vector.reduce.xor" + equalitySuffix, "reduceWithXor", selfFromThis(), elementType, {}, [](const PrimitiveVectorTypeValuePtr &self){
                        return self->reduce("^");
                    }, MethodFlags::Pure),
                }},
            });
        }

        addMethodCategories(MethodCategories{
            {"reductions", {
                makeIntrinsicMethodBindingWithSignature<AnyValuePtr (PrimitiveVectorTypeValuePtr)> ("vector.sum" + equalitySuffix, "sum", selfFromThis(), elementType, {}, [](const PrimitiveVectorTypeValuePtr &self){
                    return self->reduce("+");
                }, MethodFlags::Pure),
                makeIntrinsicMethodBindingWithSignature<AnyValuePtr (PrimitiveVectorTypeValuePtr)> ("vector.product" + equalitySuffix, "product", selfFromThis(), elementType, {}, [](const PrimitiveVectorTypeValuePtr &self){
                    return self->reduce("*");
                }, MethodFlags::Pure),
                makeIntrinsicMethodBindingWithSignature<AnyValuePtr (PrimitiveVectorTypeValuePtr)> ("vector.max" + comparisonSuffix, "max", selfFromThis(), elementType, {}, [](const PrimitiveVectorTypeValuePtr &self){
                    return self->reduce("max:");
                }, MethodFlags::Pure),
                makeIntrinsicMethodBindingWithSignature<AnyValuePtr (PrimitiveVectorTypeValuePtr)> ("vector.min" + comparisonSuffix, "min", selfFromThis(), elementType, {}, [](const PrimitiveVectorTypeValuePtr &self){
                    return self->reduce("min:");
                }, MethodFlags::Pure),
            }},
        });
    }

    // Linear algebra.
    addMethodCategories(MethodCategories{
        {"linear algebra", {
            makeIntrinsicMethodBindingWithSignature<AnyValuePtr (PrimitiveVectorTypeValuePtr, PrimitiveVectorTypeValuePtr)> ("vector.dot" + equalitySuffix, "dot:", selfFromThis(), elementType, {selfFromThis()}, [](const PrimitiveVectorTypeValuePtr &self, const PrimitiveVectorTypeValuePtr &other){
                return self->perform<PrimitiveVectorTypeValuePtr> ("*", other)->reduce("+");
            }, MethodFlags::Pure),
        }}
    });

    if(elements == 3)
    {
        addMethodCategories(MethodCategories{
            {"linear algebra", {
                makeIntrinsicMethodBindingWithSignature<PrimitiveVectorTypeValuePtr (PrimitiveVectorTypeValuePtr, PrimitiveVectorTypeValuePtr)> ("vector.cross" + equalitySuffix, "cross:", selfFromThis(), selfFromThis(), {selfFromThis()}, [=](const PrimitiveVectorTypeValuePtr &self, const PrimitiveVectorTypeValuePtr &other){
                    const auto &a1 = self->elements[0];
                    const auto &a2 = self->elements[1];
                    const auto &a3 = self->elements[2];

                    const auto &b1 = other->elements[0];
                    const auto &b2 = other->elements[1];
                    const auto &b3 = other->elements[2];

                    auto times = internSymbol("*");
                    auto subtract = internSymbol("-");

                    return withValues({
                        a2->perform<AnyValuePtr> (times, b3)->perform<AnyValuePtr>(subtract, a3->perform<AnyValuePtr> (times, b2)),
                        a3->perform<AnyValuePtr> (times, b1)->perform<AnyValuePtr>(subtract, a1->perform<AnyValuePtr> (times, b3)),
                        a1->perform<AnyValuePtr> (times, b2)->perform<AnyValuePtr>(subtract, a2->perform<AnyValuePtr> (times, b1))
                    });
                }, MethodFlags::Pure),
            }}
        });
    }

    // Unary intrinsics.
    {
        std::string scalarSelectors[] = {
            "negated", "pre--", "bitInvert", "pre-~", "sqrt",
            "abs", "sign", "floor", "ceiling"
        };

        for(const auto &scalarSelector : scalarSelectors)
        {
            auto scalarSelectorSymbol = internSymbol(scalarSelector);
            auto scalarMethod = elementType->lookupSelector(scalarSelectorSymbol);
            if(!validAnyValue(scalarMethod)->isSpecificMethod())
                continue;
            
            auto intrinsicName = scalarMethod.staticAs<SpecificMethod> ()->getIntrinsicName();
            if(validAnyValue(intrinsicName)->isUndefined())
                continue;

            auto scalarResultType = scalarMethod.staticAs<SpecificMethod> ()->getFunctionalType()->getResultType();
            auto resultType = staticObjectCast<PrimitiveVectorType> (make(scalarResultType, elements));

            addMethodCategories({
                {"arithmetic", {
                    makeIntrinsicMethodBindingWithSignature<AnyValuePtr (PrimitiveVectorTypeValuePtr)> (intrinsicName->asString(), scalarSelector, selfFromThis(), resultType, {selfFromThis()}, [=](const PrimitiveVectorTypeValuePtr &self){
                        auto result = basicMakeObject<PrimitiveVectorTypeValue> ();
                        result->type = resultType;
                        result->elements.reserve(elements);

                        for(uint32_t i = 0; i < elements; ++i)
                            result->elements.push_back(self->elements[i]->performWithArguments(scalarSelectorSymbol, {}));
                        return result;
                    }, MethodFlags::Pure),
                }}
            });
        }
    }

    // Binary intrinsics.
    {
        std::string scalarSelectors[] = {
            "+", "-", "*", "/", "//", "%", "\\\\",
            "|", "bitOr:", "&", "bitAnd:", "^", "bitXor:", "<<", ">>",

            "min:", "max:",
        };

        for(const auto &scalarSelector : scalarSelectors)
        {
            auto scalarSelectorSymbol = internSymbol(scalarSelector);
            auto scalarMethod = elementType->lookupSelector(scalarSelectorSymbol);
            if(!validAnyValue(scalarMethod)->isSpecificMethod())
                continue;
            
            auto intrinsicName = scalarMethod.staticAs<SpecificMethod> ()->getIntrinsicName();
            if(validAnyValue(intrinsicName)->isUndefined())
                continue;

            auto scalarResultType = scalarMethod.staticAs<SpecificMethod> ()->getFunctionalType()->getResultType();
            auto resultType = staticObjectCast<PrimitiveVectorType> (make(scalarResultType, elements));

            addMethodCategories({
                {"arithmetic", {
                    makeIntrinsicMethodBindingWithSignature<AnyValuePtr (PrimitiveVectorTypeValuePtr, PrimitiveVectorTypeValuePtr)> (intrinsicName->asString(), scalarSelector, selfFromThis(), resultType, {selfFromThis()}, [=](const PrimitiveVectorTypeValuePtr &self, const PrimitiveVectorTypeValuePtr &other){
                        auto result = basicMakeObject<PrimitiveVectorTypeValue> ();
                        result->type = resultType;
                        result->elements.reserve(elements);

                        for(uint32_t i = 0; i < elements; ++i)
                            result->elements.push_back(self->elements[i]->performWithArguments(scalarSelectorSymbol, {other->elements[i]}));
                        return result;
                    }, MethodFlags::Pure),
                }}
            });
        }
    }
}

bool PrimitiveVectorTypeValue::isPrimitiveVectorTypeValue() const
{
    return true;
}

AnyValuePtr PrimitiveVectorTypeValue::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitPrimitiveVectorTypeValue(selfFromThis());
}

TypePtr PrimitiveVectorTypeValue::getType() const
{
    return type;
}

std::string PrimitiveVectorTypeValue::printString() const
{
    std::ostringstream out;
    out << type->printString();
    out << '(';
    bool first = true;
    for(auto &el : elements)
    {
        if(first)
            first = false;
        else
            out << ", ";
        out << el->printString();
    }
    out << ')';

    return out.str();
}

SExpression PrimitiveVectorTypeValue::asSExpression() const
{
    SExpressionList elementsSExpr;
    elementsSExpr.elements.reserve(elements.size());
    for(auto &el : elements)
        elementsSExpr.elements.push_back(el->asSExpression());

    return SExpressionList{{
        SExpressionIdentifier{{"primitiveVector"}},
        type->asSExpression(),
        elementsSExpr
    }};
}

AnyValuePtr PrimitiveVectorTypeValue::reduce(const AnyValuePtr &selector)
{
    auto result = elements[0];
    for(size_t i = 1; i < elements.size(); ++i)
        result = result->perform<AnyValuePtr> (selector, elements[i]);
    return result;
}

AnyValuePtr PrimitiveVectorTypeValue::reduce(const std::string &selector)
{
    return reduce(internSymbol(selector));
}

} // End of namespace Environment
} // End of namespace Sysmel
