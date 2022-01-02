#include "Environment/PaddingType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/TypeVisitor.hpp"


namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<PaddingType> bootstrapTypeTypeRegistration;

PaddingTypePtr PaddingType::make(uint64_t size)
{
    static std::unordered_map<uint64_t, PaddingTypePtr> cache;

    auto it = cache.find(size);
    if(it != cache.end())
        return it->second;

    auto result = basicMakeGlobalSingletonObject<PaddingType> ();
    result->size = size;
    return result;
}

bool PaddingType::isPaddingType() const
{
    return true;
}

AnyValuePtr PaddingType::acceptTypeVisitor(const TypeVisitorPtr &typeVisitor)
{
    return typeVisitor->visitPaddingType(selfFromThis());
}

uint64_t PaddingType::getMemorySize()
{
    return size;
}

uint64_t PaddingType::getMemoryAlignment()
{
    return 1;
}

bool PaddingType::isNullableType() const
{
    return true;
}

bool PaddingType::isImmutableType()
{
    return true;
}

} // End of namespace Environment
} // End of namespace Sysmel