#include "sysmel/BootstrapEnvironment/Wrappers.hpp"
#include "sysmel/BootstrapEnvironment/LargeInteger.hpp"
#include "sysmel/BootstrapEnvironment/LiteralBooleanTrue.hpp"
#include "sysmel/BootstrapEnvironment/LiteralBooleanFalse.hpp"
#include "sysmel/BootstrapEnvironment/LiteralArray.hpp"
#include "sysmel/BootstrapEnvironment/LiteralFraction.hpp"
#include "sysmel/BootstrapEnvironment/LiteralInteger.hpp"
#include "sysmel/BootstrapEnvironment/LiteralCharacter.hpp"
#include "sysmel/BootstrapEnvironment/LiteralFloat.hpp"
#include "sysmel/BootstrapEnvironment/LiteralString.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

AnyValuePtr WrapValue<bool>::apply(bool value)
{
    if(value)
        return LiteralBooleanTrue::uniqueInstance();
    return LiteralBooleanFalse::uniqueInstance();
}

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

AnyValuePtr WrapValue<Fraction>::apply(const Fraction &value)
{
    return LiteralFraction::makeFor(value);
}

AnyValuePtr WrapValue<double>::apply(double value)
{
    return basicMakeObject<LiteralFloat> (value);
}

AnyValuePtr WrapValue<std::string>::apply(const std::string &value)
{
    return LiteralString::makeFor(value);
}

AnyValuePtr WrapValue<AnyValuePtrList>::apply(const AnyValuePtrList &content)
{
    return LiteralArray::makeFor(content);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius