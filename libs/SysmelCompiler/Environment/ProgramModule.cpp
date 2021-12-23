#include "Environment/ProgramModule.hpp"
#include "Environment/Namespace.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/SSAModule.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<ProgramModule> programModuleTypeRegistration;

ProgramModulePtr ProgramModule::create(const std::string &name)
{
    auto result = basicMakeObject<ProgramModule> ();
    result->name = internSymbol(name);
    result->activeDuring([&](){
        result->initialize();
    });
    
    return result;
}

void ProgramModule::initialize()
{
    SuperType::initialize();
    
    globalNamespace = Namespace::makeWithName(nullptr);
    globalNamespace->setParentProgramEntity(selfFromThis());
}

NamespacePtr ProgramModule::getGlobalNamespace() const
{
    return globalNamespace;
}

AnyValuePtr ProgramModule::getName() const
{
    return name;
}

bool ProgramModule::isProgramModule() const
{
    return true;
}

void ProgramModule::enqueueProgramEntitySemanticAnalysis(const ModuleDefinedProgramEntityPtr &programEntity)
{
    if(programEntitiesWithPendingAnalysisSet.find(programEntity) != programEntitiesWithPendingAnalysisSet.end())
        return;

    programEntitiesWithPendingAnalysis.push_back(programEntity);
    programEntitiesWithPendingAnalysisSet.insert(programEntity);
}

void ProgramModule::analyzeAllPendingProgramEntities()
{
    std::vector<ModuleDefinedProgramEntityPtr> toAnalyze;
    toAnalyze.swap(programEntitiesWithPendingAnalysis);
    programEntitiesWithPendingAnalysisSet.clear();
    while(!toAnalyze.empty())
    {
        for(auto &entity : toAnalyze)
            entity->ensureSemanticAnalysis();

        toAnalyze.clear();
        toAnalyze.swap(programEntitiesWithPendingAnalysis);
        programEntitiesWithPendingAnalysisSet.clear();
    }

    asSSAValueRequiredInPosition(ASTSourcePosition::empty());
}

SSAValuePtr ProgramModule::asSSAValueRequiredInPosition(const ASTSourcePositionPtr &position)
{
    if(!ssaModule)
    {
        ssaModule = basicMakeObject<SSAModule> ();
        ssaModule->setName(getValidName());
        ssaModule->setGlobalNamespace(globalNamespace->asSSAValueRequiredInPosition(position));
    }
    return ssaModule;
}

SExpression ProgramModule::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"module"}},
        name
    }};
}

SExpression ProgramModule::asFullDefinitionSExpression() const
{
    SExpressionList programEntitities;
    programEntitities.elements.reserve(registeredProgramEntities.size());
    for(auto &entity : registeredProgramEntities)
        programEntitities.elements.push_back(entity->asFullDefinitionSExpression());

    return SExpressionList{{
        SExpressionIdentifier{{"module"}},
        name,
        globalNamespace->asFullDefinitionSExpression(),
    }};
}


} // End of namespace Environment
} // End of namespace Sysmel