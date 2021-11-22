#include "sysmel/BootstrapEnvironment/EnumType.hpp"
#include "sysmel/BootstrapEnvironment/DeferredCompileTimeCodeFragment.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/CompilationError.hpp"
#include "sysmel/BootstrapEnvironment/StringUtilities.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<EnumTypeValue> EnumTypeValueTypeRegistration;

bool EnumType::isEnumType() const
{
    return true;
}

const TypePtr &EnumType::getBaseType()
{
    evaluatePendingValueTypeCodeFragments();
    return SuperType::getBaseType();
}

void EnumType::enqueuePendingValueTypeCodeFragment(const DeferredCompileTimeCodeFragmentPtr &codeFragment)
{
    pendingValueTypeCodeFragments.push_back(codeFragment);
    enqueuePendingSemanticAnalysis();
}

void EnumType::enqueuePendingValuesCodeFragment(const DeferredCompileTimeCodeFragmentPtr &codeFragment)
{
    pendingValuesCodeFragments.push_back(codeFragment);
    enqueuePendingSemanticAnalysis();
}

void EnumType::evaluatePendingValueTypeCodeFragments()
{
    while(!pendingValueTypeCodeFragments.empty())
    {
        auto toEvaluate = pendingValueTypeCodeFragments;
        pendingValueTypeCodeFragments.clear();
        for(auto &fragment : toEvaluate)
        {
            auto newBaseType = fragment->analyzeAndEvaluateAsTypeExpression();
            if(!hasEvaluatedValueType)
            {
                baseType = newBaseType;
                hasEvaluatedValueType = true;
            }
            else
            {
                // Raise an error.
                if(baseType != newBaseType)
                {
                    auto errorNode = std::make_shared<ASTSemanticErrorNode> ();
                    errorNode->sourcePosition = fragment->codeFragment->sourcePosition;
                    errorNode->errorMessage = formatString("Conflicting value type ({2} vs {3}) given for {1}.", {{
                        baseType->printString(), newBaseType->printString(), printString()
                    }});
                    errorNode->asCompilationError()->signal();
                }
            }
        }
    }

    hasEvaluatedValueType = true;
}

bool EnumTypeValue::isEnumTypeValue() const
{
    return true;
}

TypePtr EnumTypeValue::getType() const
{
    return type;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius