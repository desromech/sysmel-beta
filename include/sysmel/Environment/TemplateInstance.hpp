#ifndef SYSMEL_ENVIRONMENT_TEMPLATE_INSTANCE_HPP
#define SYSMEL_ENVIRONMENT_TEMPLATE_INSTANCE_HPP
#pragma once

#include "ModuleDefinedProgramEntity.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(TemplateInstance)
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSATemplateInstance)

struct TemplateDefinitionFragment;

/**
 * I am the base interface for most of the methods that are defined in the system.
 */
class SYSMEL_COMPILER_LIB_EXPORT TemplateInstance : public SubtypeOf<ModuleDefinedProgramEntity, TemplateInstance>
{
public:
    static constexpr char const __typeName__[] = "TemplateInstance";

    virtual bool isTemplateInstance() const override;

    virtual ProgramEntityPtr getMainTemplateInstanceChild() const override;
    virtual std::string getQualifiedName() const override;
    std::string getInstanceNameWithArguments() const;

    virtual void recordChildProgramEntityDefinition(const ProgramEntityPtr &newChild) override;
    virtual SSAValuePtr asSSAValueRequiredInPosition(const ASTSourcePositionPtr &requiredSourcePosition) override;

    const AnyValuePtr &getValue() const;
    void addArgumentBinding(const AnyValuePtr &name, const AnyValuePtr &value);
    void evaluateDefinitionFragment(const TemplateDefinitionFragment &fragment);

protected:
    ProgramEntityPtrList children;
    SSATemplateInstancePtr ssaTemplateInstance;

    bool hasEvaluatedMainDefinitionFragment = false;
    AnyValuePtr value;
    std::vector<std::pair<AnyValuePtr, AnyValuePtr>> argumentBindings;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_TEMPLATE_INSTANCE_HPP