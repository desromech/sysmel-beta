#include "Environment/Wrappers.hpp"
#include "Environment/LargeInteger.hpp"
#include "Environment/LiteralBooleanTrue.hpp"
#include "Environment/LiteralBooleanFalse.hpp"
#include "Environment/LiteralArray.hpp"
#include "Environment/LiteralFraction.hpp"
#include "Environment/LiteralInteger.hpp"
#include "Environment/LiteralCharacter.hpp"
#include "Environment/LiteralFloat.hpp"
#include "Environment/LiteralString.hpp"
#include <algorithm>

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel