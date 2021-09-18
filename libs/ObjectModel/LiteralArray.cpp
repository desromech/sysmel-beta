#include "sysmel/ObjectModel/LiteralArray.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include "sysmel/ObjectModel/Error.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace ObjectModel
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

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius