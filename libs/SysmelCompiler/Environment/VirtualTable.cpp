#include "Environment/VirtualTable.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<VirtualTable> VirtualTableTypeRegistration;

bool VirtualTable::isVirtualTable() const
{
    return true;
}

AnyValuePtr VirtualTable::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitVirtualTable(selfFromThis());
}

} // End of namespace Environment
} // End of namespace Sysmel