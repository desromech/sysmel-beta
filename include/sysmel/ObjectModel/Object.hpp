#ifndef SYSMEL_OBJECT_MODEL_OBJECT_HPP
#define SYSMEL_OBJECT_MODEL_OBJECT_HPP
#pragma once

#include <stddef.h>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

#define SYSMEL_CLASS_DECLARE(selfType, superType) \
    typedef selfType SelfType; \
    typedef superType SuperType; \
    static const char *cppClassTypeName() {return #selfType;} \
    static ObjectMethodPtrList instanceSideMethods(); \
    static ObjectMethodPtrList classSideMethods(); \
    virtual ObjectTypePtr getType() const override {return TypeSingletonForClass<selfType>::uniqueInstance(); } \
    std::shared_ptr<selfType> shared_from_this() {return std::static_pointer_cast<selfType> (superType::shared_from_this()); }

#define BEGIN_SYSMEL_INSTANCE_SIDE_METHODS(className) \
    ObjectMethodPtrList className::instanceSideMethods() { return {
#define END_SYSMEL_INSTANCE_SIDE_METHODS(className) \
    }; }

#define BEGIN_SYSMEL_CLASS_SIDE_METHODS(className) \
    ObjectMethodPtrList className::classSideMethods() { return {
#define END_SYSMEL_CLASS_SIDE_METHODS(className) \
    }; }

#define MAKE_SYSMEL_MEMBER_METHOD_WRAPPER_WITH_SELECTOR(methodName, selector) \
    makeObjectMethodWrapper(selector, &SelfType::methodName)

#define MAKE_SYSMEL_MEMBER_METHOD_WRAPPER(methodName) \
    MAKE_SYSMEL_MEMBER_METHOD_WRAPPER_WITH_SELECTOR(methodName, #methodName)

namespace SysmelMoebius
{
namespace ObjectModel
{

struct Object;
typedef std::shared_ptr<Object> ObjectPtr;
typedef std::vector<ObjectPtr> ObjectPtrList;

struct ObjectType;
typedef std::shared_ptr<ObjectType> ObjectTypePtr;
typedef std::weak_ptr<ObjectType> ObjectTypeWeakPtr;

struct ObjectMethod;
typedef std::shared_ptr<ObjectMethod> ObjectMethodPtr;
typedef std::vector<ObjectMethodPtr> ObjectMethodPtrList;

class CompilerObjectType;
typedef std::shared_ptr<CompilerObjectType> CompilerObjectTypePtr;

class CompilerObjectClass;
typedef std::shared_ptr<CompilerObjectClass> CompilerObjectClassPtr;
typedef std::weak_ptr<CompilerObjectClass> CompilerObjectClassWeakPtr;

class CompilerObjectMetaclass;
typedef std::shared_ptr<CompilerObjectMetaclass> CompilerObjectMetaclassPtr;
typedef std::weak_ptr<CompilerObjectMetaclass> CompilerObjectMetaclassWeakPtr;

class CompilerObjectTypes;

template<typename CT>
struct SharedPtrSingletonInstanceHolderForClass
{
    static std::shared_ptr<CT> value;
};

template<typename CT>
std::shared_ptr<CT> SharedPtrSingletonInstanceHolderForClass<CT>::value = std::make_shared<CT> ();

template<typename CT>
class TypeSingletonForClass
{
public:
    static CompilerObjectClassPtr classType();
    static ObjectTypePtr uniqueInstance();

private:
    static CompilerObjectClassPtr value;
};

template<typename CT>
CompilerObjectClassPtr TypeSingletonForClass<CT>::value;

template<>
class TypeSingletonForClass<void>
{
public:
    static CompilerObjectClassPtr classType()
    {
        return nullptr;
    }

    static ObjectTypePtr uniqueInstance()
    {
        return nullptr;
    }
};

/**
 * The interface for an object.
 */
struct Object : std::enable_shared_from_this<Object>
{
    typedef Object SelfType;
    typedef void SuperType;

    static ObjectMethodPtrList instanceSideMethods();
    static ObjectMethodPtrList classSideMethods();

    static const char *cppClassTypeName()
    {
        return "Object";
    }

    virtual ~Object() {}
    virtual ObjectTypePtr getType() const
    {
        return TypeSingletonForClass<Object>::uniqueInstance();
    }

    virtual ObjectPtr performWithArguments(const std::string &selector, const ObjectPtrList &arguments);
    virtual std::string asString() const;
    virtual ObjectPtr yourself();

    // Testing methods.
    virtual bool isNotNil() const;
    virtual bool isNil() const;
    virtual bool isVoid() const;
    virtual bool isBoolean() const;
    virtual bool isNumber() const;
    virtual bool isInteger() const;
    virtual bool isFloat() const;
    virtual bool isString() const;
    virtual bool isSymbol() const;

    // These methods are to simplify the implementation of the marshalling methods.
    virtual bool asBoolean8() const;
    virtual char asChar8() const;
    virtual char16_t asChar16() const;
    virtual char32_t asChar32() const;
    virtual uint8_t asUInt8() const;
    virtual int8_t asInt8() const;
    virtual uint16_t asUInt16() const;
    virtual int16_t asInt16() const;
    virtual uint32_t asUInt32() const;
    virtual int32_t asInt32() const;
    virtual uint64_t asUInt64() const;
    virtual int64_t asInt64() const;
    virtual float asFloat32() const;
    virtual double asFloat64() const;
    virtual std::string asNativeString() const;
};

/**
 * The interface for looking and invoking an object method.
 */
struct ObjectMethod : Object
{
    SYSMEL_CLASS_DECLARE(ObjectMethod, Object);

    virtual const std::string &getSelector() const = 0;
    virtual size_t getArgumentCount() const = 0;
    virtual ObjectPtr evaluateWithArguments(const ObjectPtr &receiver, const ObjectPtrList &arguments) const = 0;
};

/**
 * The interface for looking and invoking a compiler object method.
 */
struct ObjectType : Object
{
    SYSMEL_CLASS_DECLARE(ObjectType, Object);

    virtual ObjectTypePtr getSuperType() const = 0;
    virtual ObjectMethodPtr lookupSelector(const std::string &selector) const = 0;

    virtual void addMethods(const ObjectMethodPtrList &methods) = 0;
    virtual void addMethod(const ObjectMethodPtr &method) = 0;
};

/**
 * A standard object type
 */
class CompilerObjectType : public ObjectType
{
public:
    SYSMEL_CLASS_DECLARE(CompilerObjectType, ObjectType);

    virtual ObjectTypePtr getSuperType() const override;
    virtual ObjectMethodPtr lookupSelector(const std::string &selector) const override;

    virtual void addMethods(const ObjectMethodPtrList &methods) override;
    virtual void addMethod(const ObjectMethodPtr &method) override;

    ObjectTypeWeakPtr superType;
    std::unordered_map<std::string, ObjectMethodPtr> methodDict;
};

class CompilerObjectClass : public CompilerObjectType
{
public:
    typedef Object SelfType;
    typedef CompilerObjectType SuperType;

    static ObjectMethodPtrList instanceSideMethods();
    static ObjectMethodPtrList classSideMethods();

    static const char *cppClassTypeName()
    {
        return "CompilerObjectClass";
    }

    virtual ObjectTypePtr getType() const override;
    virtual std::string asString() const override;

    std::string name;
    CompilerObjectMetaclassPtr metaClass;
};

class CompilerObjectMetaclass : public CompilerObjectType
{
public:
    SYSMEL_CLASS_DECLARE(CompilerObjectClass, CompilerObjectType);

    virtual std::string asString() const override;

    CompilerObjectClassWeakPtr thisClass;
};

/**
 * An undefinedObject.
 */
struct UndefinedObject : Object
{
    SYSMEL_CLASS_DECLARE(UndefinedObject, Object);

    virtual bool isNil() const override;

    static ObjectPtr uniqueInstance()
    {
        return SharedPtrSingletonInstanceHolderForClass<UndefinedObject>::value;
    }


    virtual std::string asString() const override
    {
        return "nil";
    }
};

/**
 * A literal boolean true value.
 */
struct LiteralVoid : Object
{
    SYSMEL_CLASS_DECLARE(LiteralVoid, Object);

    virtual bool isVoid() const override;

    static ObjectPtr uniqueInstance()
    {
        return SharedPtrSingletonInstanceHolderForClass<LiteralVoid>::value;
    }

    virtual std::string asString() const override
    {
        return "void";
    }
};

/**
 * A literal boolean value.
 */
struct LiteralBoolean : Object
{
    SYSMEL_CLASS_DECLARE(LiteralBoolean, Object);

    virtual bool isBoolean() const override;
};

/**
 * A literal boolean true value.
 */
struct LiteralTrue : LiteralBoolean
{
    SYSMEL_CLASS_DECLARE(LiteralTrue, LiteralBoolean);

    virtual bool asBoolean8() const override;

    static ObjectPtr uniqueInstance()
    {
        return SharedPtrSingletonInstanceHolderForClass<LiteralTrue>::value;
    }

    virtual std::string asString() const override
    {
        return "true";
    }
};

/**
 * A literal boolean false value.
 */
struct LiteralFalse : LiteralBoolean
{
    SYSMEL_CLASS_DECLARE(LiteralFalse, LiteralBoolean);

    virtual bool asBoolean8() const override;

    static ObjectPtr uniqueInstance()
    {
        return SharedPtrSingletonInstanceHolderForClass<LiteralFalse>::value;
    }

    virtual std::string asString() const override
    {
        return "false";
    }
};

/**
 * A literal number value.
 */
struct LiteralNumber : Object
{
    SYSMEL_CLASS_DECLARE(LiteralNumber, Object);
};

/**
 * A literal character value.
 */
struct LiteralCharacter : Object
{
    SYSMEL_CLASS_DECLARE(LiteralCharacter, Object);

    LiteralCharacter(char32_t v)
        : value(v) {}

    char32_t value;
};

/**
 * A literal integer value.
 */
struct LiteralInteger : LiteralNumber
{
    SYSMEL_CLASS_DECLARE(LiteralInteger, LiteralNumber);
};

/**
 * A literal integer value.
 */
struct LiteralSignedInteger : LiteralInteger
{
    SYSMEL_CLASS_DECLARE(LiteralSignedInteger, LiteralInteger);

    LiteralSignedInteger(int64_t v)
        : value(v) {}

    virtual std::string asString() const override;

    virtual uint64_t asUInt64() const override;
    virtual int64_t asInt64() const override;
    virtual float asFloat32() const override;
    virtual double asFloat64() const override;

    int64_t value;
};

/**
 * A literal integer value.
 */
struct LiteralUnsignedInteger : LiteralInteger
{
    SYSMEL_CLASS_DECLARE(LiteralUnsignedInteger, LiteralInteger);

    LiteralUnsignedInteger(uint64_t v)
        : value(v) {}

    virtual std::string asString() const override;

    virtual uint64_t asUInt64() const override;
    virtual int64_t asInt64() const override;
    virtual float asFloat32() const override;
    virtual double asFloat64() const override;

    uint64_t value;
};

/**
 * A literal float value.
 */
struct LiteralFloat : LiteralNumber
{
    SYSMEL_CLASS_DECLARE(LiteralFloat, LiteralNumber);

    LiteralFloat(double v)
        : value(v) {}

    virtual std::string asString() const override;

    virtual float asFloat32() const override;
    virtual double asFloat64() const override;

    double value;
};

/**
 * A literal string
 */
struct LiteralString : Object
{
    SYSMEL_CLASS_DECLARE(LiteralString, Object);

    LiteralString(const std::string &v)
        : value(v) {}

    virtual bool isString() const override;
    virtual std::string asString() const override;
    virtual std::string asNativeString() const override;

    std::string value;
};

/**
 * A literal symbol
 */
struct LiteralSymbol : LiteralString
{
    SYSMEL_CLASS_DECLARE(LiteralSymbol, LiteralString);

    LiteralSymbol(const std::string &v)
        : SuperType(v) {}

    virtual bool isSymbol() const override;
};

// ObjectToCpp
template<typename T>
struct ObjectToCppObject;

template<typename T>
struct ObjectToCppObject<std::shared_ptr<T>>
{
    std::shared_ptr<T> operator()(const ObjectPtr &object)
    {
        if(object && object->isNil())
            return nullptr;

        return std::static_pointer_cast<T> (object);
    }
};

template<>
struct ObjectToCppObject<std::string>
{
    std::string operator()(const ObjectPtr &object)
    {
        return object->asNativeString();
    }
};

template<>
struct ObjectToCppObject<bool>
{
    bool operator()(const ObjectPtr &object)
    {
        return object->asBoolean8();
    }
};

template<>
struct ObjectToCppObject<char>
{
    char operator()(const ObjectPtr &object)
    {
        return object->asChar8();
    }
};

template<>
struct ObjectToCppObject<char16_t>
{
    char16_t operator()(const ObjectPtr &object)
    {
        return object->asChar16();
    }
};

template<>
struct ObjectToCppObject<char32_t>
{
    char32_t operator()(const ObjectPtr &object)
    {
        return object->asChar32();
    }
};

template<>
struct ObjectToCppObject<uint8_t>
{
    uint8_t operator()(const ObjectPtr &object)
    {
        return object->asUInt8();
    }
};

template<>
struct ObjectToCppObject<int8_t>
{
    int8_t operator()(const ObjectPtr &object)
    {
        return object->asInt8();
    }
};

template<>
struct ObjectToCppObject<uint16_t>
{
    uint16_t operator()(const ObjectPtr &object)
    {
        return object->asUInt16();
    }
};

template<>
struct ObjectToCppObject<int16_t>
{
    int16_t operator()(const ObjectPtr &object)
    {
        return object->asInt16();
    }
};

template<>
struct ObjectToCppObject<uint32_t>
{
    uint32_t operator()(const ObjectPtr &object)
    {
        return object->asUInt32();
    }
};

template<>
struct ObjectToCppObject<int32_t>
{
    int32_t operator()(const ObjectPtr &object)
    {
        return object->asInt32();
    }
};

template<>
struct ObjectToCppObject<uint64_t>
{
    uint64_t operator()(const ObjectPtr &object)
    {
        return object->asUInt64();
    }
};

template<>
struct ObjectToCppObject<int64_t>
{
    int64_t operator()(const ObjectPtr &object)
    {
        return object->asInt64();
    }
};

template<>
struct ObjectToCppObject<float>
{
    float operator()(const ObjectPtr &object)
    {
        return object->asFloat32();
    }
};

template<>
struct ObjectToCppObject<double>
{
    double operator()(const ObjectPtr &object)
    {
        return object->asFloat64();
    }
};

template<typename T>
struct ObjectToCppObject<const T &> : ObjectToCppObject<T> {};

// CppToObject
template<typename T>
struct CppObjectToObject;

template<typename T>
struct CppObjectToObject<std::shared_ptr<T>>
{
    ObjectPtr operator()(const std::shared_ptr<T> &v)
    {
        if(!v)
            return UndefinedObject::uniqueInstance();
        return v;
    }
};

template<>
struct CppObjectToObject<bool>
{
    ObjectPtr operator()(bool value)
    {
        return value ? LiteralTrue::uniqueInstance() : LiteralFalse::uniqueInstance();
    }
};

template<>
struct CppObjectToObject<int64_t>
{
    ObjectPtr operator()(int64_t value)
    {
        return std::make_shared<LiteralSignedInteger> (value);
    }
};

template<>
struct CppObjectToObject<uint64_t>
{
    ObjectPtr operator()(uint64_t value)
    {
        return std::make_shared<LiteralUnsignedInteger> (value);
    }
};

template<>
struct CppObjectToObject<char32_t>
{
    ObjectPtr operator()(char32_t value)
    {
        return std::make_shared<LiteralCharacter> (value);
    }
};

template<>
struct CppObjectToObject<double>
{
    ObjectPtr operator()(double value)
    {
        return std::make_shared<LiteralFloat> (value);
    }
};

template<>
struct CppObjectToObject<int32_t> : CppObjectToObject<int64_t> {};

template<>
struct CppObjectToObject<int16_t> : CppObjectToObject<int64_t> {};

template<>
struct CppObjectToObject<int8_t> : CppObjectToObject<int64_t> {};

template<>
struct CppObjectToObject<uint32_t> : CppObjectToObject<uint64_t> {};

template<>
struct CppObjectToObject<uint16_t> : CppObjectToObject<uint64_t> {};

template<>
struct CppObjectToObject<uint8_t> : CppObjectToObject<uint64_t> {};

template<>
struct CppObjectToObject<char> : CppObjectToObject<char32_t> {};

template<>
struct CppObjectToObject<char16_t> : CppObjectToObject<char32_t> {};

template<>
struct CppObjectToObject<float> : CppObjectToObject<double> {};

template<>
struct CppObjectToObject<std::string>
{
    ObjectPtr operator()(const std::string &value)
    {
        return std::make_shared<LiteralString> (value);
    }
};

template<>
struct CppObjectToObject<const char *>
{
    ObjectPtr operator()(const std::string &value)
    {
        return std::make_shared<LiteralString> (value);
    }
};

template<typename CT, typename RT, typename MFT, typename ...Args>
class ObjectMemberMethodWrapper : public ObjectMethod
{
public:
    typedef std::shared_ptr<CT> ReceiverType;
    typedef RT ResultType;
    typedef MFT MemberFunctionPointerType;

    ObjectMemberMethodWrapper(const std::string &cselector, MemberFunctionPointerType cfunctionPointer)
        : selector(cselector), functionPointer(cfunctionPointer) {}

    virtual const std::string &getSelector() const override
    {
        return selector;
    }

    virtual size_t getArgumentCount() const override
    {
        return sizeof...(Args);
    }

    virtual ObjectPtr evaluateWithArguments(const ObjectPtr &receiver, const ObjectPtrList &arguments) const override
    {
        size_t nextIndex = 0;
        auto &castedReceiver = *ObjectToCppObject<ReceiverType>()(receiver);
        if constexpr(std::is_same<ResultType, void>::value)
        {
            (castedReceiver.*functionPointer) (
                ObjectToCppObject<Args> ()(arguments[nextIndex++]) ...
            );
            return LiteralVoid::uniqueInstance();
        }
        else
        {
            return CppObjectToObject<ResultType> ()(
                (castedReceiver.*functionPointer) (
                    ObjectToCppObject<Args> ()(arguments[nextIndex++]) ...
                )
            );
        }
    }

private:
    std::string selector;
    MemberFunctionPointerType functionPointer;
};

template<typename F>
struct ObjectMethodWrapper;

template<typename CT, typename RT, typename ...Args>
struct ObjectMethodWrapper<RT (CT::*) (Args...)>
{
    typedef ObjectMemberMethodWrapper<CT, RT, RT (CT::*) (Args...), Args...> type;
};

template<typename CT, typename RT, typename ...Args>
struct ObjectMethodWrapper<RT (CT::*) (Args...) const>
{
    typedef ObjectMemberMethodWrapper<CT, RT, RT (CT::*) (Args...) const, Args...> type;
};

template<typename FT>
inline ObjectMethodPtr makeObjectMethodWrapper(const std::string &selector, const FT &f)
{
    return std::make_shared<typename ObjectMethodWrapper<FT>::type> (selector, f);
}

template<typename CT>
inline CompilerObjectClassPtr TypeSingletonForClass<CT>::classType()
{
    if(value)
        return value;

    value = std::make_shared<CompilerObjectClass> ();
    value->name = CT::cppClassTypeName();
    value->metaClass = std::make_shared<CompilerObjectMetaclass> ();

    auto superType = TypeSingletonForClass<typename CT::SuperType>::classType();
    value->superType = superType;
    if(superType)
        value->metaClass->superType = superType->getType();
    else
        value->metaClass->superType = TypeSingletonForClass<CompilerObjectClass>::classType(); // Short circuit

    value->addMethods(CT::instanceSideMethods());
    value->metaClass->addMethods(CT::classSideMethods());

    return value;
}

template<typename CT>
inline ObjectTypePtr TypeSingletonForClass<CT>::uniqueInstance()
{
    return classType();
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_OBJECT_MODEL_OBJECT_HPP
