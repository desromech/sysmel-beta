#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include <assert.h>

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

const StaticBootstrapDefinedTypeMetadataList &getBootstrapDefinedTypeMetadataList()
{
    return *getOrCreateMetadataList();
}

void registerBootstrapDefinedTypeMetadata(StaticBootstrapDefinedTypeMetadata *metadata)
{
    assert(metadata->bootstrapTypeID == 0);
    const auto &list = getOrCreateMetadataList();
    list->push_back(metadata);
    metadata->bootstrapTypeID = list->size();
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius