#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

typedef std::shared_ptr<StaticBootstrapDefinedTypeMetadataList> StaticBootstrapDefinedTypeMetadataListPtr;

static StaticBootstrapDefinedTypeMetadataListPtr getOrCreateMetadataList()
{
    static StaticBootstrapDefinedTypeMetadataListPtr singleton = std::make_shared<StaticBootstrapDefinedTypeMetadataList> ();
    return singleton;
}

const std::vector<const StaticBootstrapDefinedTypeMetadata*> &getBootstrapDefinedTypeMetadataList()
{
    return *getOrCreateMetadataList();
}

void registerBootstrapDefinedTypeMetadata(const StaticBootstrapDefinedTypeMetadata *metadata)
{
    getOrCreateMetadataList()->push_back(metadata);
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius