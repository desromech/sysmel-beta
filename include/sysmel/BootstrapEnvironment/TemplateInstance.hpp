#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TEMPLATE_INSTANCE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TEMPLATE_INSTANCE_HPP
#pragma once

#include "ModuleDefinedProgramEntity.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(TemplateInstance)

struct TemplateDefinitionFragment;

/**
 * I am the base interface for most of the methods that are defined in the system.
 */
class TemplateInstance : public SubtypeOf<ModuleDefinedProgramEntity, TemplateInstance>
{
public:
    static constexpr char const __typeName__[] = "TemplateInstance";

    virtual bool isTemplateInstance() const override;

    virtual void recordChildProgramEntityDefinition(const ProgramEntityPtr &newChild) override;

    const AnyValuePtr &getValue() const;
    void addArgumentBinding(const AnyValuePtr &name, const AnyValuePtr &value);
    void evaluateDefinitionFragment(const TemplateDefinitionFragment &fragment);

protected:
    ProgramEntityPtrList children;

    bool hasEvaluatedMainDefinitionFragment = false;
    AnyValuePtr value;
    std::vector<std::pair<AnyValuePtr, AnyValuePtr>> argumentBindings;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TEMPLATE_INSTANCE_HPP