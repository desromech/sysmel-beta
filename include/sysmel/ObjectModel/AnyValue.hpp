#ifndef SYSMEL_COMPILER_OBJECT_MODEL_OBJECT_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_OBJECT_HPP
#pragma once

#include <memory>
#include <string>
#include <vector>

namespace SysmelMoebius
{
namespace ObjectModel
{

class AnyValue;
typedef std::shared_ptr<AnyValue> AnyValuePtr;

class Type;
typedef std::shared_ptr<Type> TypePtr;

typedef std::pair<std::string, AnyValuePtr> MethodBinding;
typedef std::vector<MethodBinding> MethodBindings;

typedef std::pair<std::string, MethodBindings> MethodCategory;
typedef std::vector<MethodCategory> MethodCategories;

struct StaticBootstrapDefinedTypeMetadata
{
    const StaticBootstrapDefinedTypeMetadata *supertype;
    std::string typeName;
    MethodCategories instanceMethods;
    MethodCategories typeMethods;
};

TypePtr getBootstrapDefinedTypeWithMetadata(const StaticBootstrapDefinedTypeMetadata *metadata);

template<typename T>
struct StaticBootstrapDefinedTypeMetadataFor
{
    static const StaticBootstrapDefinedTypeMetadata metadata;

    static const StaticBootstrapDefinedTypeMetadata *get()
    {
        return &metadata;
    }
};

template<typename T>
const StaticBootstrapDefinedTypeMetadata StaticBootstrapDefinedTypeMetadataFor<T>::metadata = StaticBootstrapDefinedTypeMetadata{
    StaticBootstrapDefinedTypeMetadataFor<typename T::SuperType>::get(),
    T::__typeName__,
    T::__instanceMethods__(),
    T::__typeMethods__()
};

template<>
struct StaticBootstrapDefinedTypeMetadataFor<void>
{
    static const StaticBootstrapDefinedTypeMetadata *get()
    {
        return nullptr;
    }
};

template<typename T>
struct StaticBootstrapDefinedTypeFor
{
    static TypePtr get()
    {
        return getBootstrapDefinedTypeWithMetadata(StaticBootstrapDefinedTypeMetadataFor<T>::get());
    }
};

template<typename SuT, typename ST>
class SubtypeOf : public SuT
{
public:
    typedef SuT SuperType;
    typedef ST SelfType;

    static constexpr char const __typeName__[] = "";

    static MethodCategories __instanceMethods__()
    {
        return MethodCategories{};
    }

    static MethodCategories __typeMethods__()
    {
        return MethodCategories{};
    }

    static TypePtr __staticType__()
    {
        return StaticBootstrapDefinedTypeFor<SelfType>::get();
    }
    
    virtual TypePtr getType() const override
    {
        return StaticBootstrapDefinedTypeFor<SelfType>::get();
    }

    std::shared_ptr<SelfType> shared_from_this()
    {
        return std::static_pointer_cast<SelfType> (SuperType::shared_from_this());
    }
};

/**
 * I am the base interface for any value that is passed through the interpreter.
 */
class AnyValue : public std::enable_shared_from_this<AnyValue>
{
public:
    typedef void SuperType;
    typedef AnyValue SelfType;

    static constexpr char const __typeName__[] = "AnyValue";
    static MethodCategories __instanceMethods__();
    static MethodCategories __typeMethods__();

    static TypePtr __staticType__()
    {
        return StaticBootstrapDefinedTypeFor<SelfType>::get();
    }

    /// Retrieves the type of the object.
    virtual TypePtr getType() const
    {
        return StaticBootstrapDefinedTypeFor<SelfType>::get();
    }

    virtual ~AnyValue();

    /// Generic method for initializing the object.
    virtual void initialize();

    /// Is this object a type?.
    virtual bool isType() const;

    /// Is this object the a bootstrap defined type?.
    virtual bool isBootstrapType() const;

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

    /// This method evaluates the specific message in the receiver with the specific arguments.
    virtual AnyValuePtr runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver);

    /// Evaluates the requested message with the given arguments.
    virtual AnyValuePtr performWithArguments(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments);
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_OBJECT_HPP