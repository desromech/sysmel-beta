#ifndef SYSMEL_ENVIRONMENT_SSA_CODE_GENERATION_BACKEND_HPP
#define SYSMEL_ENVIRONMENT_SSA_CODE_GENERATION_BACKEND_HPP
#pragma once

#include "CompilerObject.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSACodeGenerationBackend);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ProgramModule);

enum class SSACodeGenerationOutputMode : uint8_t
{
    Executable = 0,
    SharedLibrary,
    Plugin,
    ObjectFile,
    Assembly,
};

/**
 * I am the base interface for a SSA based code generation backend
 */
class SSACodeGenerationBackend : public SubtypeOf<CompilerObject, SSACodeGenerationBackend>
{
public:
    static constexpr char const __typeName__[] = "SSACodeGenerationBackend";

    static SSACodeGenerationBackendPtr makeNativeBackend();
    static SSACodeGenerationBackendPtr makeSpirVBackend();

    virtual void setOutputMode(SSACodeGenerationOutputMode newOutputMode);
    virtual void setOutputFileName(const std::string &newOutputFileName);
    virtual void setEmitTargetIR(bool newEmitTargetIR);
    virtual bool processAndWriteProgramModule(const ProgramModulePtr &programModule);

protected:
    SSACodeGenerationOutputMode outputMode = SSACodeGenerationOutputMode::Executable;
    std::string outputFileName;
    bool emitTargetIR = false;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_CODE_GENERATION_BACKEND_HPP