#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <assert.h>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

typedef std::shared_ptr<StaticBootstrapDefinedTypeMetadataList> StaticBootstrapDefinedTypeMetadataListPtr;

static StaticBootstrapDefinedTypeMetadataListPtr getOrCreateMetadataList()
{
    static StaticBootstrapDefinedTypeMetadataListPtr singleton = basicMakeObject<StaticBootstrapDefinedTypeMetadataList> ();
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
    metadata->bootstrapTypeID = list->size()*2 + 1;
    list->push_back(metadata);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius