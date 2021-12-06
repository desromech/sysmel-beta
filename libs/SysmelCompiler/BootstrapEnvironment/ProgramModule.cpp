#include "sysmel/BootstrapEnvironment/ProgramModule.hpp"
#include "sysmel/BootstrapEnvironment/Namespace.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<ProgramModule> programModuleTypeRegistration;

ProgramModulePtr ProgramModule::create(const std::string &name)
{
    auto result = basicMakeObject<ProgramModule> ();
    result->name = name;
    result->activeDuring([&](){
        result->initialize();
    });
    
    return result;
}

void ProgramModule::initialize()
{
    SuperType::initialize();
    
    globalNamespace = Namespace::makeWithName(nullptr);
    globalNamespace->registerInCurrentModule();
}

NamespacePtr ProgramModule::getGlobalNamespace() const
{
    return globalNamespace;
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


} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius