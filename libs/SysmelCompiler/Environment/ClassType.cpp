#include "Environment/ClassType.hpp"
#include "Environment/DeferredCompileTimeCodeFragment.hpp"
#include "Environment/Error.hpp"
#include "Environment/PointerLikeType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/ASTSemanticErrorNode.hpp"
#include "Environment/CompilationError.hpp"
#include "Environment/StringUtilities.hpp"
#include "Environment/LiteralValueVisitor.hpp"
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

AggregateTypeLayoutPtr ClassType::makeLayoutInstance()
{
    return basicMakeObject<AggregateTypeSequentialLayout> ();
}

AnyValuePtr ClassType::basicNewValue()
{
    auto sequentialLayout = getLayout().staticAs<AggregateTypeSequentialLayout> ();
    assert(sequentialLayout);

    const auto &slotTypes = sequentialLayout->getSlotTypes();

    auto result = basicMakeObject<ClassTypeValue> ();
    result->type = selfFromThis();
    result->slots.reserve(slotTypes.size());

    for(const auto &slotType : slotTypes)
        result->slots.push_back(slotType->basicNewValue());

    // TODO: Set the vtable/typeinfo values.

    return result;
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
    elementsSExpr.elements.reserve(slots.size());
    for(auto &el : slots)
        elementsSExpr.elements.push_back(el->asSExpression());

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