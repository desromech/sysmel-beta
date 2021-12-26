#include "Environment/CVarArg.hpp"
#include "Environment/PrimitiveBooleanType.hpp"
#include "Environment/PrimitiveCharacterType.hpp"
#include "Environment/PrimitiveIntegerType.hpp"
#include "Environment/PrimitiveFloatType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include <algorithm>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<CVarArg> CVarArgTypeRegistration;

TypePtr Type::getCVarArgsType()
{
    return CVarArg::__staticType__()->withParams();
}

bool Type::isCVarArgType() const
{
    return this == CVarArg::__staticType__().get();
}

bool Type::isCVarArgsType() const
{
    return this == getCVarArgsType().get();
}

CVarArgPtr CVarArg::make(const AnyValuePtr &value)
{
    auto result = basicMakeObject<CVarArg> ();
    result->value = value; 
    return result;
}

AnyValuePtrList CVarArg::__constructors__()
{
    return AnyValuePtrList{
            makeIntrinsicConstructor<CVarArgPtr (TypePtr, UInt8Ptr)> ("cvar.arg.zeroext32", +[](const TypePtr &, const UInt8Ptr &value){
                return make(value);
            }),
            makeIntrinsicConstructor<CVarArgPtr (TypePtr, UInt16Ptr)> ("cvar.arg.zeroext32", +[](const TypePtr &, const UInt16Ptr &value){
                return make(value);
            }),
            makeIntrinsicConstructor<CVarArgPtr (TypePtr, UInt32Ptr)> ("cvar.arg.identity", +[](const TypePtr &, const UInt32Ptr &value){
                return make(value);
            }),
            makeIntrinsicConstructor<CVarArgPtr (TypePtr, UInt64Ptr)> ("cvar.arg.identity", +[](const TypePtr &, const UInt64Ptr &value){
                return make(value);
            }),
            makeIntrinsicConstructor<CVarArgPtr (TypePtr, Int8Ptr)> ("cvar.arg.signext32", +[](const TypePtr &, const Int8Ptr &value){
                return make(value);
            }),
            makeIntrinsicConstructor<CVarArgPtr (TypePtr, Int16Ptr)> ("cvar.arg.signext32", +[](const TypePtr &, const Int16Ptr &value){
                return make(value);
            }),
            makeIntrinsicConstructor<CVarArgPtr (TypePtr, Int32Ptr)> ("cvar.arg.identity", +[](const TypePtr &, const Int32Ptr &value){
                return make(value);
            }),
            makeIntrinsicConstructor<CVarArgPtr (TypePtr, Int64Ptr)> ("cvar.arg.identity", +[](const TypePtr &, const Int64Ptr &value){
                return make(value);
            }),
            makeIntrinsicConstructor<CVarArgPtr (TypePtr, Char8Ptr)> ("cvar.arg.zeroext32", +[](const TypePtr &, const Char8Ptr &value){
                return make(value);
            }),
            makeIntrinsicConstructor<CVarArgPtr (TypePtr, Char16Ptr)> ("cvar.arg.zeroext32", +[](const TypePtr &, const Char16Ptr &value){
                return make(value);
            }),
            makeIntrinsicConstructor<CVarArgPtr (TypePtr, Char32Ptr)> ("cvar.arg.identity", +[](const TypePtr &, const Char32Ptr &value){
                return make(value);
            }),

            makeIntrinsicConstructor<CVarArgPtr (TypePtr, Float16Ptr)> ("cvar.arg.float64", +[](const TypePtr &, const Float16Ptr &value){
                return make(value);
            }),
            makeIntrinsicConstructor<CVarArgPtr (TypePtr, Float32Ptr)> ("cvar.arg.float64", +[](const TypePtr &, const Float32Ptr &value){
                return make(value);
            }),
            makeIntrinsicConstructor<CVarArgPtr (TypePtr, Float64Ptr)> ("cvar.arg.identity", +[](const TypePtr &, const Float64Ptr &value){
                return make(value);
            }),

        };
}

bool CVarArg::isCVarArg() const
{
    return true;
}

} // End of namespace Environment
} // End of namespace Sysmel