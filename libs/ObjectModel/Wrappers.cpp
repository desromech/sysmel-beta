#include "sysmel/ObjectModel/Wrappers.hpp"
#include "sysmel/ObjectModel/LargeInteger.hpp"
#include "sysmel/ObjectModel/LiteralInteger.hpp"
#include "sysmel/ObjectModel/LiteralFloat.hpp"
#include "sysmel/ObjectModel/LiteralString.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace ObjectModel
{

AnyValuePtr WrapValue<uint32_t>::apply(uint32_t value)
{
    return LiteralInteger::makeFor(LargeInteger{value});
}

AnyValuePtr WrapValue<int32_t>::apply(int32_t value)
{
    return LiteralInteger::makeFor(LargeInteger{value});
}

AnyValuePtr WrapValue<uint64_t>::apply(uint64_t value)
{
    return LiteralInteger::makeFor(LargeInteger{value});
}

AnyValuePtr WrapValue<int64_t>::apply(int64_t value)
{
    return LiteralInteger::makeFor(LargeInteger{value});
}

AnyValuePtr WrapValue<char32_t>::apply(char32_t value)
{
    return LiteralInteger::makeForCharacter(value);
}

AnyValuePtr WrapValue<LargeInteger>::apply(const LargeInteger &value)
{
    return LiteralInteger::makeFor(value);
}

AnyValuePtr WrapValue<double>::apply(double value)
{
    return std::make_shared<LiteralFloat> (value);
}

AnyValuePtr WrapValue<std::string>::apply(const std::string &value)
{
    return LiteralString::makeFor(value);
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius