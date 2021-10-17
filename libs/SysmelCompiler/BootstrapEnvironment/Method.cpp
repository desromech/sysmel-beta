#include "sysmel/BootstrapEnvironment/Method.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"


namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<Method> methodTypeRegistration;

bool Method::isMethod() const
{
    return true;
}

AnyValuePtr Method::getName() const
{
    return name;
}

void Method::setName(const AnyValuePtr &newName)
{
    name = newName;
}

TypePtr Method::getExpectedTypeForAnalyzingArgumentWithIndex(size_t argumentIndex)
{
    (void)argumentIndex;
    return nullptr;
}

MethodPatternMatchingResult Method::matchPatternForRunWithIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver)
{
    (void)selector;
    (void)arguments;
    (void)receiver;
    return MethodPatternMatchingResult{};
}

MethodPatternMatchingResult Method::matchPatternForAnalyzingMessageSendNode(const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    (void)node;
    (void)semanticAnalyzer;
    return MethodPatternMatchingResult{};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius