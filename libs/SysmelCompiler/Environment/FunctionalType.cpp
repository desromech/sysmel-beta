#include "Environment/FunctionalType.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/SubclassResponsibility.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
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

std::string FunctionalType::printString() const
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
            out << arguments[i]->printString();
        }
    }
    out << ") => ";
    out << result->printString();

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

FunctionTypePtr FunctionalType::copyWithResultType(const TypePtr &newResultType)
{
    (void)newResultType;
    SysmelSelfSubclassResponsibility();
}

bool FunctionalType::matchesSignature(const TypePtr &testReceiverType, const TypePtrList &testArgumentTypes, const TypePtr &testResultType) const
{
    return getReceiverType() == testReceiverType && arguments == testArgumentTypes && result == testResultType;
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