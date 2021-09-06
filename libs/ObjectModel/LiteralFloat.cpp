#include "sysmel/ObjectModel/LiteralFloat.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include <algorithm>
#include <sstream>

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<LiteralFloat> literalFloatTypeRegistration;

bool LiteralFloat::isLiteralFloat() const
{
    return true;
}

std::string LiteralFloat::printString() const
{
    std::ostringstream out;
    out << value;
    return out.str();
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius