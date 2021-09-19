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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius