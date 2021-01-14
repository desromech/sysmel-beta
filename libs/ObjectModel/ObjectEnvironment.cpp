#include "sysmel/ObjectModel/ObjectEnvironment.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(ObjectEnvironment)
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER_WITH_SELECTOR(getCompilationTarget, "compilationTarget"),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER_WITH_SELECTOR(getSchema, "schema"),
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(ObjectEnvironment)
END_SYSMEL_CLASS_SIDE_METHODS()


StandardObjectEnvironment::StandardObjectEnvironment()
{
    schema = std::make_shared<Schema> ();
}

CompilationTargetPtr StandardObjectEnvironment::getCompilationTarget() const
{
    return compilationTarget;
}

SchemaPtr StandardObjectEnvironment::getSchema() const
{
    return schema;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius
