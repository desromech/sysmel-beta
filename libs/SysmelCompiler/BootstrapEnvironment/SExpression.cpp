#include "sysmel/BootstrapEnvironment/SExpression.hpp"
#include "sysmel/BootstrapEnvironment/StringUtilities.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

struct SExpressionPrinterVisitor
{
    void operator()(std::nullptr_t) 
    {
        out << "nil";
    }

    void operator()(bool value) 
    {
        out << (value ? "true" : "false");
    }

    void operator()(SExpressionVoid)
    {
        out << "void";
    }

    void operator()(const LargeInteger &value)
    {
        out << value;
    }

    void operator()(const Fraction &value)
    {
        out << value;
    }

    void operator()(double value)
    {
        out << value;
    }

    void operator()(char32_t value)
    {
        out << '\'';
        formatUtf32Character(value, out);
        out << '\'';
    }

    void operator()(const std::string &value)
    {
        out << formatStringLiteral(value);
    }

    void operator()(const SExpressionIdentifier &value)
    {
        out << value.value;
    }

    void operator()(const SExpressionSymbol &value)
    {
        out << formatSymbolLiteral(value.value);
    }

    void operator()(const SExpressionList &value)
    {
        out << '(';
        size_t listStartIndex = 0;
        if(prettyPrint)
        {
            bool hasSeenSourcePosition = false;
            for(; listStartIndex < value.elements.size();)
            {
                const auto &element = value.elements[listStartIndex];
                if(shouldPrintInitialInline(element))
                {
                    if(hasSeenSourcePosition)
                    {
                        if(!isSourceCodePosition(element))
                            break;
                    }
                    else
                    {
                        hasSeenSourcePosition = isSourceCodePosition(element);
                    }

                    if(listStartIndex > 0)
                        out << ' ';
                    std::visit(*this, element);
                    ++listStartIndex;
                }
                else
                {
                    break;
                }
            }
        }

        if(shouldPrintInline(value))
        {
            for(size_t i = listStartIndex; i < value.elements.size(); ++i)
            {
                if(i > 0)
                    out << ' ';
                std::visit(*this, value.elements[i]);
            }
        }
        else
        {
            ++indentation;
            for(size_t i = listStartIndex; i < value.elements.size(); ++i)
            {
                newIndentedLine();
                std::visit(*this, value.elements[i]);
            }
            --indentation;
        }

        out << ')';
    }

    bool isSourceCodePosition(const SExpression &value)
    {
        if(!std::holds_alternative<SExpressionList> (value))
            return false;
        
        const auto &list = std::get<SExpressionList> (value);
        return list.elements.size() > 0
            && std::holds_alternative<SExpressionIdentifier> (list.elements[0])
            && std::get<SExpressionIdentifier> (list.elements[0]).value == "position";
    }

    bool shouldPrintInitialInline(const SExpression &value)
    {
        if(!prettyPrint)
            return false;

        if(std::holds_alternative<SExpressionList> (value))
            return isSourceCodePosition(value);
        
        return true;
    }

    bool shouldPrintInline(const SExpressionList &list)
    {
        if(!prettyPrint)
            return true;
        if(list.elements.size() <= 1)
            return true;

        for(auto &el : list.elements)
        {
            if(std::holds_alternative<SExpressionList> (el))
            {
                if(!isAlwaysInlineList(std::get<SExpressionList> (el)))
                    return false;
            }
        }

        return true;
    }

    bool isAlwaysInlineList(const SExpressionList &list)
    {
        return list.elements.size() <= 2;
    }

    void newIndentedLine()
    {
        out << "\n";
        for(int i = 0; i < indentation; ++i)
            out << "  ";
    }

    bool prettyPrint = false;
    int indentation = 0;
    std::ostringstream out;
};

std::string sexpressionToString(const SExpression &sexpr)
{
    SExpressionPrinterVisitor visitor;
    std::visit(visitor, sexpr);
    return visitor.out.str();
}

std::string sexpressionToPrettyString(const SExpression &sexpr)
{
    SExpressionPrinterVisitor visitor;
    visitor.prettyPrint = true;
    std::visit(visitor, sexpr);
    return visitor.out.str();
}
} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius