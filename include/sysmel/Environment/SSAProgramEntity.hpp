#ifndef SYSMEL_ENVIRONMENT_SSA_PROGRAM_ENTITY_HPP
#define SYSMEL_ENVIRONMENT_SSA_PROGRAM_ENTITY_HPP
#pragma once

#include "SSAGlobalValue.hpp"
#include "ProgramEntityVisibility.hpp"
#include "DllLinkageMode.hpp"
#include "ExternalLanguageMode.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAProgramEntity : public SubtypeOf<SSAGlobalValue, SSAProgramEntity>
{
public:
    static constexpr char const __typeName__[] = "SSAProgramEntity";

    virtual bool isSSAProgramEntity() const override;

    virtual AnyValuePtr getName() const override;
    void setName(const AnyValuePtr &newName);
    std::string getValidNameString() const;

    virtual void addChild(const SSAProgramEntityPtr &child);
    virtual SSAProgramEntityPtr getMainTemplateInstanceChild() const;

    SSAProgramEntityPtr getParent() const;
    void setParent(const SSAProgramEntityPtr &newParent);

    ProgramEntityVisibility getVisiblity() const;
    void setVisibility(ProgramEntityVisibility newVisibility);

    DllLinkageMode getDllLinkageMode() const;
    void setDllLinkageMode(DllLinkageMode newDllLinkageMode);

    ExternalLanguageMode getExternalLanguageMode() const;
    void setExternalLanguageMode(ExternalLanguageMode newExternalLanguageMode);

protected:
    AnyValuePtr name;
    SSAProgramEntityWeakPtr parent;
    ProgramEntityVisibility visibility = ProgramEntityVisibility::Default;
    DllLinkageMode dllLinkageMode = DllLinkageMode::None;
    ExternalLanguageMode externalLanguageMode = ExternalLanguageMode::None;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_PROGRAM_ENTITY_HPP