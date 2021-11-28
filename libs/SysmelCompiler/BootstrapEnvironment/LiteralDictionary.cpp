#include "sysmel/BootstrapEnvironment/LiteralDictionary.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<LiteralDictionary> literalArrayRegistration;

LiteralDictionaryPtr LiteralDictionary::makeFor(const AnyValuePtrList &elements)
{
    auto result = basicMakeObject<LiteralDictionary> ();
    result->elements = elements;
    return result;
}

MethodCategories LiteralDictionary::__instanceMethods__()
{
    return MethodCategories{
    };
}

bool LiteralDictionary::isLiteralDictionary() const
{
    return true;
}

AnyValuePtrList LiteralDictionary::unwrapAsArray() const
{
    return elements;
}

std::string LiteralDictionary::printString() const
{
    std::ostringstream out;
    out << "LiteralDictionary(";
    bool isFirst = true;
    for(auto &el : elements)
    {
        if(isFirst)
            isFirst = false;
        else
            out << ", ";
        out << el->printString();
    }
    out << ')';
    return out.str();
}

SExpression LiteralDictionary::asSExpression() const
{
    SExpressionList result;
    result.elements.reserve(1 + elements.size());
    result.elements.push_back(SExpressionIdentifier{{"dictionary"}});
    for(const auto &el : elements)
        result.elements.push_back(el ? el->asSExpression() : nullptr);
    return result;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius