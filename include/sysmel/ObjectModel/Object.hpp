#ifndef SYSMEL_COMPILER_OBJECT_MODEL_OBJECT_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_OBJECT_HPP

#include <memory>
#include <string>
#include <vector>

namespace SysmelMoebius
{
namespace ObjectModel
{

class Object;
typedef std::shared_ptr<Object> ObjectPtr;

/**
 * I am the base interface for compile time object that is passed through the interpreter.
 */
class Object : public std::enable_shared_from_this<Object>
{
public:
    virtual ~Object();

    /// Is this object a literal value?
    virtual bool isLiteralValue() const;

    /// Is this object a literal number?
    virtual bool isLiteralNumber() const;

    /// Is this object a literal integer?
    virtual bool isLiteralInteger() const;

    /// Is this object a literal positive integer?
    virtual bool isLiteralPositiveInteger() const;

    /// Is this object a literal negative integer?
    virtual bool isLiteralNegativeInteger() const;

    /// Is this object a literal character?
    virtual bool isLiteralCharacter() const;

    /// Is this object a literal fraction?
    virtual bool isLiteralFraction() const;

    /// Is this object a literal float?
    virtual bool isLiteralFloat() const;

    /// Is this object a literal string?
    virtual bool isLiteralString() const;

    /// Is this object a literal symbol?
    virtual bool isLiteralSymbol() const;

    /// Is this object a literal boolean?
    virtual bool isLiteralBoolean() const;

    /// Is this object a literal boolean true?
    virtual bool isLiteralBooleanTrue() const;

    /// Is this object a literal boolean false?
    virtual bool isLiteralBooleanFalse() const;

    /// Is this object a literal void?
    virtual bool isLiteralVoid() const;

    /// Convert the object into a string.
    virtual std::string asString() const;

    /// Convert the object into a string for printing purposes.
    virtual std::string printString() const;

    /// Evaluates the requested message with the given arguments.
    virtual ObjectPtr performWithArguments(const ObjectPtr &selector, const std::vector<ObjectPtr> &arguments);
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_OBJECT_HPP