#include "sysmel/BootstrapEnvironment/LiteralArray.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/Error.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<LiteralArray> literalArrayRegistration;

TypePtr WrapperTypeFor<AnyValuePtrList>::apply()
{
    return LiteralArray::__staticType__();
}

LiteralArrayPtr LiteralArray::makeFor(const AnyValuePtrList &content)
{
    auto result = std::make_shared<LiteralArray> ();
    result->content = content;
    return result;
}

MethodCategories LiteralArray::__instanceMethods__()
{
    return MethodCategories{
        {"transforming", {
            // =
            makeMethodBinding("select:", &SelfType::select, MethodFlags::Pure),
            makeMethodBinding("collect:", &SelfType::collect, MethodFlags::Pure),
        }},
    };
}

bool LiteralArray::isLiteralArray() const
{
    return true;
}

AnyValuePtrList LiteralArray::unwrapAsArray() const 
{
    return content;
}

std::string LiteralArray::printString() const
{
    std::ostringstream out;
    out << "#(";
    for(size_t i = 0; i < content.size(); ++i)
    {
        if(i > 0)
            out << ' ';
        out << content[i]->printString();
    }
    out << ')';
    return out.str();
}

SExpression LiteralArray::asSExpression() const
{
    SExpressionList result;
    result.elements.reserve(content.size());
    for(const auto &el : content)
        result.elements.push_back(el ? el->asSExpression() : nullptr);
    return result;
}

LiteralArrayPtr LiteralArray::select(const AnyValuePtr &aBlock) const
{
    auto result = std::make_shared<LiteralArray> ();
    for(auto & el : content)
    {
        if(validAnyValue(aBlock->applyWithArguments({el}))->unwrapAsBoolean())
            result->content.push_back(el);
    }

    return result;
}

LiteralArrayPtr LiteralArray::collect(const AnyValuePtr &aBlock) const
{
    auto result = std::make_shared<LiteralArray> ();
    result->content.reserve(content.size());
    for(auto & el : content)
        result->content.push_back(aBlock->applyWithArguments({el}));

    return result;

}


} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius