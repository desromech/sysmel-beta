#include "sysmel/BootstrapEnvironment/PrimitiveBooleanType.hpp"
#include "sysmel/BootstrapEnvironment/LiteralBoolean.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<PrimitiveBooleanType> PrimitiveBooleanTypeTypeRegistration;
static BootstrapTypeRegistration<Boolean8> Boolean8TypeRegistration;

bool PrimitiveBooleanType::isPrimitiveBooleanTypeValue() const
{
    return true;
}

Boolean8Ptr Boolean8::make(bool value)
{
    auto result = std::make_shared<Boolean8> ();
    result->value = value;
    return result;
}

MethodCategories Boolean8::__instanceMethods__()
{
    return MethodCategories{};
}

MethodCategories Boolean8::__typeMacroMethods__()
{
    return MethodCategories{};
}

AnyValuePtrList Boolean8::__constructors__()
{
    return AnyValuePtrList{
            makeConstructor<Boolean8Ptr (TypePtr, LiteralBooleanPtr)> (+[](const TypePtr &, const LiteralBooleanPtr &value){
                return make(value->unwrapAsBoolean());
            }, MethodFlags::Explicit | MethodFlags::Pure),
        };
}

bool Boolean8::__canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value)
{
    return value && value->isLiteralBoolean();
}

AnyValuePtr Boolean8::__instantiateWithLiteralValue__(const AnyValuePtr &value)
{
    assert(value->isLiteralBooleanTrue() || value->isLiteralBooleanFalse());
    return make(value->isLiteralBooleanTrue());
}

bool Boolean8::unwrapAsBoolean() const
{
    return value;
}

std::string Boolean8::asString() const
{
    return value ? "true" : "false";
}

std::string Boolean8::printString() const
{
    return std::string("Boolean8(") + (value ? "true" : "false") + ")";
}

SExpression Boolean8::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"boolean8"}},
        value
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius