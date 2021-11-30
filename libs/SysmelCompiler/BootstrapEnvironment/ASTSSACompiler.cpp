#include "sysmel/BootstrapEnvironment/ASTSSACompiler.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

#include "sysmel/BootstrapEnvironment/ASTNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSequenceNode.hpp"

#include "sysmel/BootstrapEnvironment/ASTValueAsVoidTypeConversionNode.hpp"

#include "sysmel/BootstrapEnvironment/CompiledMethod.hpp"
#include "sysmel/BootstrapEnvironment/SSAFunction.hpp"
#include "sysmel/BootstrapEnvironment/SSACodeRegion.hpp"
#include "sysmel/BootstrapEnvironment/SSABasicBlock.hpp"
#include "sysmel/BootstrapEnvironment/SSABuilder.hpp"

#include "sysmel/BootstrapEnvironment/SSAConstantLiteralValue.hpp"

#include "sysmel/BootstrapEnvironment/ControlFlowUtilities.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTSSACompiler> ASTSSACompilerTypeRegistration;

SSAValuePtr ASTSSACompiler::visitNodeForValue(const ASTNodePtr &node)
{
    auto oldPosition = builder->getSourcePosition();
    builder->setSourcePosition(node->sourcePosition);
    auto value = doWithEnsure([&](){
        return visitNode(node);
    }, [&]{
        builder->setSourcePosition(oldPosition);
    });

    assert(value->isSSAValue());
    return staticObjectCast<SSAValue> (value);
}

void ASTSSACompiler::compileMethodBody(const CompiledMethodPtr &method, const SSAFunctionPtr &ssaFunction, const ASTNodePtr &node)
{
    currentMethod = method;
    currentSSAFunction = ssaFunction;
    currentCodeRegion = ssaFunction->getMainCodeRegion();

    builder = basicMakeObject<SSABuilder> ();
    builder->setSourcePosition(node->sourcePosition);
    builder->setCodeRegion(currentCodeRegion);
    builder->makeBasicBlockHere();
    
    auto resultValue = visitNodeForValue(node);
    if(!builder->isLastTerminator())
        builder->returnFromFunction(resultValue);
}

AnyValuePtr ASTSSACompiler::visitLiteralValueNode(const ASTLiteralValueNodePtr &node)
{
    return node->value->asSSAValueRequiredInPosition(builder->getSourcePosition());
}

AnyValuePtr ASTSSACompiler::visitSequenceNode(const ASTSequenceNodePtr &node)
{
    SSAValuePtr result;
    for(auto &expression : node->expressions)
    {
        result = visitNodeForValue(expression);
        if(builder->isLastTerminator())
            break;
    }

    if(!result)
        result = builder->literal(getVoidConstant());
    return result;
}

AnyValuePtr ASTSSACompiler::visitValueAsVoidTypeConversionNode(const ASTValueAsVoidTypeConversionNodePtr &node)
{
    visitNodeForValue(node->expression);
    return builder->literal(getVoidConstant());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius