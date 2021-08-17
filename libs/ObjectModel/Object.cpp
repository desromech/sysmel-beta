#include "sysmel/ObjectModel/Object.hpp"
#include "sysmel/ObjectModel/Error.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace ObjectModel
{
Object::~Object()
{
}

bool Object::isLiteralValue() const
{
    return false;
}

bool Object::isLiteralNumber() const
{
    return false;
}

bool Object::isLiteralInteger() const
{
    return false;
}

bool Object::isLiteralPositiveInteger() const
{
    return false;
}

bool Object::isLiteralNegativeInteger() const
{
    return false;
}

bool Object::isLiteralCharacter() const
{
    return false;
}

bool Object::isLiteralFraction() const
{
    return false;
}

bool Object::isLiteralFloat() const
{
    return false;
}

bool Object::isLiteralString() const
{
    return false;
}

bool Object::isLiteralSymbol() const
{
    return false;
}

bool Object::isLiteralBoolean() const
{
    return false;
}

bool Object::isLiteralBooleanTrue() const
{
    return false;
}

bool Object::isLiteralBooleanFalse() const
{
    return false;
}

bool Object::isLiteralVoid() const
{
    return false;
}

std::string Object::asString() const
{
    return printString();
}

std::string Object::printString() const
{
    return "an Object";
}

ObjectPtr Object::performWithArguments(const ObjectPtr &selector, const std::vector<ObjectPtr> &arguments)
{
    throw MessageNotUnderstood("Message " + selector->printString() + " is not understood by " + printString() + ".");
}
} // End of namespace ObjectModel
} // End of namespace SysmelMoebius