#include "Environment/ClassType.hpp"
#include "Environment/DeferredCompileTimeCodeFragment.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/ASTSemanticErrorNode.hpp"
#include "Environment/CompilationError.hpp"
#include "Environment/StringUtilities.hpp"

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

bool ClassTypeValue::isClassTypeValue() const
{
    return true;
}

} // End of namespace Environment
} // End of namespace Sysmel