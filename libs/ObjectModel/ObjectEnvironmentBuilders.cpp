#include "sysmel/ObjectModel/ObjectEnvironmentBuilders.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(ObjectEnvironmentTypeMetaBuilder)
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(ObjectEnvironmentTypeMetaBuilder)
END_SYSMEL_CLASS_SIDE_METHODS()

ObjectPtr ObjectEnvironmentTypeMetaBuilder::performWithArguments(const std::string &selector, const ObjectPtrList &)
{
    auto schema = objectEnvironment->getSchema();
    auto existent = schema->getTypeDefinitionNamed(selector);
    if(existent)
        return existent;

    auto newType = std::make_shared<SchemaTypeDefinition> ();
    newType->name = selector;

    auto newMetaType = std::make_shared<SchemaTypeDefinition> ();
    newType->metaType = newMetaType;
    newMetaType->baseType = newType;
    newMetaType->flags = uint32_t(SchemaTypeFlags::IsMetaType);

    schema->addTypeDefinition(newType);
    schema->addTypeDefinition(newMetaType);
    return newType;
}


} // End of namespace ObjectModel
} // End of namespace SysmelMoebius
