#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/Assert.hpp"

namespace Sysmel
{
namespace Environment
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
    sysmelAssert(metadata->bootstrapTypeID == 0);
    const auto &list = getOrCreateMetadataList();
    metadata->bootstrapTypeID = list->size()*2 + 1;
    list->push_back(metadata);
}

} // End of namespace Environment
} // End of namespace Sysmel