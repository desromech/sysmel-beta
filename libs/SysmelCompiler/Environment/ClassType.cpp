#include "Environment/ClassType.hpp"
#include "Environment/DeferredCompileTimeCodeFragment.hpp"
#include "Environment/Error.hpp"
#include "Environment/PointerLikeType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/ASTSemanticErrorNode.hpp"
#include "Environment/CompilationError.hpp"
#include "Environment/StringUtilities.hpp"
#include "Environment/SpecificMethod.hpp"
#include "Environment/VirtualTable.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include "Environment/TypeVisitor.hpp"
#include "Environment/AggregateTypeSequentialLayout.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ClassTypeValue> ClassTypeValueTypeRegistration;

bool ClassType::isClassType() const
{
    return true;
}

TypePtr ClassType::getSupertype() const
{
    evaluatePendingSuperclassDefinitions();
    return SuperType::getSupertype();
}

void ClassType::evaluateAllPendingCodeFragments()
{
    evaluatePendingSuperclassDefinitions();
    evaluateAllPendingBodyBlockCodeFragments();
}

void ClassType::evaluateAllPendingBodyBlockCodeFragments()
{
    evaluatePendingSuperclassDefinitions();
    SuperType::evaluateAllPendingBodyBlockCodeFragments();
}

void ClassType::evaluatePendingSuperclassDefinitions() const
{
    while(!pendingSuperclassCodeFragments.empty())
    {
        auto fragmentsToProcess = pendingSuperclassCodeFragments;
        pendingSuperclassCodeFragments.clear();
        for(auto &fragment : fragmentsToProcess)
        {
            auto newSupertype = fragment->analyzeAndEvaluateAsTypeExpression();
            if(!hasEvaluatedSuperclassCodeFragment)
            {
                supertype = newSupertype;
                getType()->setSupertype(newSupertype->getType());
                hasEvaluatedSuperclassCodeFragment = true;
            }
            else
            {
                // Raise an error
                if(supertype != newSupertype)
                {
                    auto errorNode = basicMakeObject<ASTSemanticErrorNode> ();
                    errorNode->sourcePosition = fragment->codeFragment->sourcePosition;
                    errorNode->errorMessage = formatString("Conflicting super classes ({2} vs {3}) given for {1}.", {{
                        supertype->printString(), newSupertype->printString(), printString()
                    }});
                    errorNode->asCompilationError()->signal();
                }

            }
        }
    }

    hasEvaluatedSuperclassCodeFragment = true;
}

void ClassType::enqueuePendingSuperclassCodeFragment(const DeferredCompileTimeCodeFragmentPtr &codeFragment)
{
    pendingSuperclassCodeFragments.push_back(codeFragment);
    enqueuePendingSemanticAnalysis();
}

void ClassType::computeObjectLifetimeTriviality()
{
    if(supertype && supertype->isAggregateType())
    {
        hasTrivialInitialization_ = supertype->hasTrivialInitialization();
        hasTrivialInitializationCopyingFrom_ = supertype->hasTrivialInitializationCopyingFrom();
        hasTrivialInitializationMovingFrom_ = supertype->hasTrivialInitializationMovingFrom();
        hasTrivialFinalization_ = supertype->hasTrivialFinalization();
        hasTrivialAssignCopyingFrom_ = supertype->hasTrivialAssignCopyingFrom();
        hasTrivialAssignMovingFrom_ = supertype->hasTrivialAssignMovingFrom();
    }
    SuperType::computeObjectLifetimeTriviality();
}

void ClassType::buildLayout()
{
    evaluateAllPendingBodyBlockCodeFragments();

    layout = makeLayoutInstance();
    hasBuiltLayout = true;

    // Add the super class layout.
    auto st = getSupertype();
    if(st->isClassType() || st->isStructureType())
    {
        auto supertypeLayout = st.staticAs<AggregateTypeWithFields> ()->getLayout();
        layout.staticAs<AggregateTypeSequentialLayout> ()->addInheritance(staticObjectCast<AggregateTypeSequentialLayout> (supertypeLayout));
    }

    // Add my layout.
    layout->beginGroup();
    layout->addVirtualMethods(virtualMethods);
    for(auto &field : fields)
        layout->addFieldVariable(field);
    layout->finishGroup();

    const auto &vtable = layout.staticAs<AggregateTypeSequentialLayout>()->getVirtualTable();
    if(vtable)
        vtable->ownerType = selfFromThis();
}

AggregateTypeLayoutPtr ClassType::makeLayoutInstance()
{
    return basicMakeObject<AggregateTypeSequentialLayout> ();
}

AggregateTypeValuePtr ClassType::makeRawValueInstance()
{
    return basicMakeObject<ClassTypeValue> ();
}

AnyValuePtr ClassType::basicNewValue()
{
    auto sequentialLayout = getLayout().staticAs<AggregateTypeSequentialLayout> ();
    sysmelAssert(sequentialLayout);

    const auto &slotTypes = sequentialLayout->getSlotTypes();

    auto result = basicMakeObject<ClassTypeValue> ();
    result->type = selfFromThis();
    result->slots.reserve(slotTypes.size());

    for(const auto &slotType : slotTypes)
        result->slots.push_back(slotType->basicNewValue());

    // Set the vtable, if it exists.
    auto vtable = sequentialLayout->getVirtualTable();
    if(vtable)
        result->slots[sequentialLayout->getVirtualTableSlotIndex()] = vtable;

    return result;
}

AnyValuePtr ClassType::acceptTypeVisitor(const TypeVisitorPtr &visitor)
{
    return visitor->visitClassType(selfFromThis());
}

bool ClassType::canHaveVirtualMethods() const
{
    return true;
}

SpecificMethodPtr ClassType::lookupParentOverridenMethod(const AnyValuePtr &selector, bool hasConstReceiver, const TypePtrList &argumentTypes)
{
    auto candidate = lookupSelector(selector);
    if(candidate)
    {
        auto result = candidate->asMethodMatchingDefinitionSignature(true, hasConstReceiver, argumentTypes, nullptr);
        if(result && result->isSpecificMethod())
            return staticObjectCast<SpecificMethod> (result);
    }

    auto st = getSupertype();
    if(st)
        return st->lookupParentOverridenMethod(selector, hasConstReceiver, argumentTypes);
    return nullptr;
}

void ClassType::addVirtualMethod(const SpecificMethodPtr &virtualMethod)
{
    if(!!hasBuiltLayout)
        signalNewWithMessage<Error> (formatString("Cannot add virtual methods to {0} after the type layout is built.", {printString()}));
    virtualMethods.push_back(virtualMethod);
}

const SpecificMethodPtrList &ClassType::getVirtualMethods() const
{
    return virtualMethods;
}

void ClassType::ensureVirtualTableLayoutComputation()
{
    // Get the layout for building the vtable.
    getLayout();
}

bool ClassTypeValue::isClassTypeValue() const
{
    return true;
}

AnyValuePtr ClassTypeValue::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitClassTypeValue(selfFromThis());
}

SExpression ClassTypeValue::asSExpression() const
{
    SExpressionList elementsSExpr;
    auto layout = getType().staticAs<ClassType> ()->getLayout().staticAs<AggregateTypeSequentialLayout> ();
    const auto &nonPaddingSlotIndices = layout->getNonPaddingSlotIndices();
    elementsSExpr.elements.reserve(nonPaddingSlotIndices.size());
    for(auto &slotIndex : nonPaddingSlotIndices)
        elementsSExpr.elements.push_back(validAnyValue(slots[slotIndex])->asSExpression());

    return SExpressionList{{
        SExpressionIdentifier{{"struct"}},
        type->asSExpression(),
        elementsSExpr
    }};
}

TypePtr ClassTypeValue::getType() const
{
    return type;
}

} // End of namespace Environment
} // End of namespace Sysmel