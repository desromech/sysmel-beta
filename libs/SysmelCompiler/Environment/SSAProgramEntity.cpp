#include "Environment/SSAProgramEntity.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/UnsupportedOperation.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSAProgramEntity> SSAProgramEntityTypeRegistration;

bool SSAProgramEntity::isSSAProgramEntity() const
{
    return true;
}

AnyValuePtr SSAProgramEntity::getName() const
{
    return name;
}

void SSAProgramEntity::setName(const AnyValuePtr &newName)
{
    name = newName;
}

std::string SSAProgramEntity::getValidNameString() const
{
    return validAnyValue(name)->asString();
}

void SSAProgramEntity::addChild(const SSAProgramEntityPtr &)
{
    signalNew<UnsupportedOperation> ();
}

SSAProgramEntityPtr SSAProgramEntity::getMainTemplateInstanceChild() const
{
    return nullptr;
}

SSAProgramEntityPtr SSAProgramEntity::getParent() const
{
    return parent.lock();
}

void SSAProgramEntity::setParent(const SSAProgramEntityPtr &newParent)
{
    parent = newParent;
}

ProgramEntityVisibility SSAProgramEntity::getVisiblity() const
{
    return visibility;
}

void SSAProgramEntity::setVisibility(ProgramEntityVisibility newVisibility)
{
    visibility = newVisibility;
}

DllLinkageMode SSAProgramEntity::getDllLinkageMode() const
{
    return dllLinkageMode;
}

void SSAProgramEntity::setDllLinkageMode(DllLinkageMode newDllLinkageMode)
{
    dllLinkageMode = newDllLinkageMode;
}

ExternalLanguageMode SSAProgramEntity::getExternalLanguageMode() const
{
    return externalLanguageMode;
}

void SSAProgramEntity::setExternalLanguageMode(ExternalLanguageMode newExternalLanguageMode)
{
    externalLanguageMode = newExternalLanguageMode;
}

} // End of namespace Environment
} // End of namespace Sysmel