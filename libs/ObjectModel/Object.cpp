#include "sysmel/ObjectModel/Object.hpp"
#include <sstream>
#include <stdexcept>

namespace SysmelMoebius
{
namespace ObjectModel
{

//==============================================================================
// Object
//==============================================================================

BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(Object)
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER_WITH_SELECTOR(getType, "__typeOf"),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER_WITH_SELECTOR(getType, "class"),

    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(yourself),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(asString),

    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(isNil),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(isNotNil),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(isVoid),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(isBoolean),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(isNumber),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(isInteger),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(isFloat),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(isString),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(isSymbol),

    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(asBoolean8),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(asChar8),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(asChar16),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(asChar32),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(asUInt8),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(asInt8),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(asUInt16),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(asInt16),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(asUInt32),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(asInt32),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(asUInt64),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(asInt64),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(asFloat32),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(asFloat64),
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(Object)
END_SYSMEL_CLASS_SIDE_METHODS()


ObjectPtr Object::yourself()
{
    return shared_from_this();
}

std::string Object::asString() const
{
    return "an " + getType()->asString();
}

bool Object::isNotNil() const
{
    return !isNil();
}

bool Object::isNil() const
{
    return false;
}
bool Object::isVoid() const
{
    return false;
}

bool Object::isBoolean() const
{
    return false;
}

bool Object::isNumber() const
{
    return false;
}

bool Object::isInteger() const
{
    return false;
}

bool Object::isFloat() const
{
    return false;
}

bool Object::isString() const
{
    return false;
}

bool Object::isSymbol() const
{
    return false;
}

bool Object::asBoolean8() const
{
    throw std::runtime_error("Invalid compiler object cast to bool.");
}

char Object::asChar8() const
{
    throw std::runtime_error("Invalid compiler object cast to char.");
}

char16_t Object::asChar16() const
{
    throw std::runtime_error("Invalid compiler object cast to char16_t.");
}

char32_t Object::asChar32() const
{
    throw std::runtime_error("Invalid compiler object cast to char132_t.");
}

uint8_t Object::asUInt8() const
{
    throw std::runtime_error("Invalid compiler object cast to uint8_t.");
}

int8_t Object::asInt8() const
{
    throw std::runtime_error("Invalid compiler object cast to int8_t.");
}

uint16_t Object::asUInt16() const
{
    throw std::runtime_error("Invalid compiler object cast to uint16_t.");
}

int16_t Object::asInt16() const
{
    throw std::runtime_error("Invalid compiler object cast to int16_t.");
}

uint32_t Object::asUInt32() const
{
    throw std::runtime_error("Invalid compiler object cast to uint32_t.");
}

int32_t Object::asInt32() const
{
    throw std::runtime_error("Invalid compiler object cast to int32_t.");
}

uint64_t Object::asUInt64() const
{
    throw std::runtime_error("Invalid compiler object cast to uint64_t.");
}

int64_t Object::asInt64() const
{
    throw std::runtime_error("Invalid compiler object cast to int64_t.");
}

float Object::asFloat32() const
{
    throw std::runtime_error("Invalid compiler object cast to float.");
}

double Object::asFloat64() const
{
    throw std::runtime_error("Invalid compiler object cast to double");
}

std::string Object::asNativeString() const
{
    throw std::runtime_error("Invalid compiler object cast to std::string.");
}

ObjectPtr Object::performWithArguments(const std::string &selector, const ObjectPtrList &arguments)
{
    auto method = getType()->lookupSelector(selector);
    if(!method)
        throw std::runtime_error("Failed to lookup selector #" + selector + ".");
    return method->evaluateWithArguments(shared_from_this(), arguments);
}

//==============================================================================
// ObjectType
//==============================================================================

BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(ObjectType)
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER_WITH_SELECTOR(getSuperType, "superType"),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER_WITH_SELECTOR(getSuperType, "superclass"),
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER_WITH_SELECTOR(lookupSelector, "lookupSelector:"),
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(ObjectType)
END_SYSMEL_CLASS_SIDE_METHODS()

//==============================================================================
// CompilerObjectType
//==============================================================================

BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(CompilerObjectType)
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(CompilerObjectType)
END_SYSMEL_CLASS_SIDE_METHODS()

ObjectTypePtr CompilerObjectType::getSuperType() const
{
    return superType.lock();
}

ObjectMethodPtr CompilerObjectType::lookupSelector(const std::string &selector) const
{
    auto it = methodDict.find(selector);
    if(it != methodDict.end())
        return it->second;

    auto s = superType.lock();
    return s ? s->lookupSelector(selector) : nullptr;
}

void CompilerObjectType::addMethods(const std::vector<ObjectMethodPtr> &methods)
{
    for(auto &method : methods)
        addMethod(method);
}

void CompilerObjectType::addMethod(const ObjectMethodPtr &method)
{
    methodDict[method->getSelector()] = method;
}

//==============================================================================
// CompilerObjectClass
//==============================================================================
BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(CompilerObjectClass)
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(CompilerObjectClass)
END_SYSMEL_CLASS_SIDE_METHODS()

ObjectTypePtr CompilerObjectClass::getType() const
{
    return metaClass;
}

std::string CompilerObjectClass::asString() const
{
    return name;
}

//==============================================================================
// CompilerObjectMetaclass
//==============================================================================

BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(CompilerObjectMetaclass)
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(CompilerObjectMetaclass)
END_SYSMEL_CLASS_SIDE_METHODS()

std::string CompilerObjectMetaclass::asString() const
{
    auto thisClassValue = thisClass.lock();
    if(thisClassValue)
        return thisClassValue->asString() + " type";
    return SuperType::asString();
}
//==============================================================================
// ObjectMethod
//==============================================================================

BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(ObjectMethod)
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(ObjectMethod)
END_SYSMEL_CLASS_SIDE_METHODS()

//==============================================================================
// UndefinedObject
//==============================================================================

BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(UndefinedObject)
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(UndefinedObject)
END_SYSMEL_CLASS_SIDE_METHODS()

bool UndefinedObject::isNil() const
{
    return true;
}

//==============================================================================
// LiteralVoid
//==============================================================================

BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(LiteralVoid)
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(LiteralVoid)
END_SYSMEL_CLASS_SIDE_METHODS()

bool LiteralVoid::isVoid() const
{
    return true;
}

//==============================================================================
// LiteralBoolean
//==============================================================================

BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(LiteralBoolean)
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(LiteralBoolean)
END_SYSMEL_CLASS_SIDE_METHODS()

bool LiteralBoolean::isBoolean() const
{
    return true;
}

//==============================================================================
// LiteralTrue
//==============================================================================

BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(LiteralTrue)
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(LiteralTrue)
END_SYSMEL_CLASS_SIDE_METHODS()

bool LiteralTrue::asBoolean8() const
{
    return true;
}

//==============================================================================
// LiteralFalse
//==============================================================================

BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(LiteralFalse)
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(LiteralFalse)
END_SYSMEL_CLASS_SIDE_METHODS()

bool LiteralFalse::asBoolean8() const
{
    return false;
}

//==============================================================================
// LiteralCharacter
//==============================================================================

BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(LiteralCharacter)
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(LiteralCharacter)
END_SYSMEL_CLASS_SIDE_METHODS()

//==============================================================================
// LiteralNumber
//==============================================================================

BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(LiteralNumber)
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(LiteralNumber)
END_SYSMEL_CLASS_SIDE_METHODS()

//==============================================================================
// LiteralInteger
//==============================================================================

BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(LiteralInteger)
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(LiteralInteger)
END_SYSMEL_CLASS_SIDE_METHODS()

//==============================================================================
// LiteralSignedInteger
//==============================================================================

BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(LiteralSignedInteger)
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(LiteralSignedInteger)
END_SYSMEL_CLASS_SIDE_METHODS()

std::string LiteralSignedInteger::asString() const
{
    std::ostringstream out;
    out << value << std::ends;
    return out.str();
}

uint64_t LiteralSignedInteger::asUInt64() const
{
    if(value < 0)
        return SuperType::asUInt64();
    return value;
}

int64_t LiteralSignedInteger::asInt64() const
{
    return value;
}

float LiteralSignedInteger::asFloat32() const
{
    return value;
}

double LiteralSignedInteger::asFloat64() const
{
    return value;
}
//==============================================================================
// LiteralUnsignedInteger
//==============================================================================

BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(LiteralUnsignedInteger)
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(LiteralUnsignedInteger)
END_SYSMEL_CLASS_SIDE_METHODS()

std::string LiteralUnsignedInteger::asString() const
{
    std::ostringstream out;
    out << value << std::ends;
    return out.str();
}

uint64_t LiteralUnsignedInteger::asUInt64() const
{
    return value;
}

int64_t LiteralUnsignedInteger::asInt64() const
{
    if(value > uint64_t(INT64_MAX))
        return SuperType::asInt64();

    return value;
}

float LiteralUnsignedInteger::asFloat32() const
{
    return value;
}

double LiteralUnsignedInteger::asFloat64() const
{
    return value;
}

//==============================================================================
// LiteralFloat
//==============================================================================

BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(LiteralFloat)
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(LiteralFloat)
END_SYSMEL_CLASS_SIDE_METHODS()

std::string LiteralFloat::asString() const
{
    std::ostringstream out;
    out << value << std::ends;
    return out.str();
}

float LiteralFloat::asFloat32() const
{
    return value;
}

double LiteralFloat::asFloat64() const
{
    return value;
}

//==============================================================================
// LiteralString
//==============================================================================

BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(LiteralString)
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(LiteralString)
END_SYSMEL_CLASS_SIDE_METHODS()

bool LiteralString::isString() const
{
    return true;
}

std::string LiteralString::asString() const
{
    return value;
}

std::string LiteralString::asNativeString() const
{
    return value;
}

//==============================================================================
// LiteralSymbol
//==============================================================================

BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(LiteralSymbol)
END_SYSMEL_INSTANCE_SIDE_METHODS()

BEGIN_SYSMEL_CLASS_SIDE_METHODS(LiteralSymbol)
END_SYSMEL_CLASS_SIDE_METHODS()

bool LiteralSymbol::isSymbol() const
{
    return true;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius
