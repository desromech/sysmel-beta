#include "sysmel/ObjectModel/LanguageSupport.hpp"
#include "sysmel/ObjectModel/ASTNode.hpp"
#include "sysmel/ObjectModel/Error.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include <fstream>
#include <iostream>

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<LanguageSupport> LanguageSupportTypeRegistration;

static std::string readContentFromFileNamed(const std::string &fileName)
{
    if(fileName == "-")
    {
        return std::string(std::istreambuf_iterator<char> (std::cin), std::istreambuf_iterator<char> ());
    }

    std::ifstream in(fileName);
    if(!in.good())
    {
        throw Error("Failed to read file named: " + fileName);
    }

    return std::string(std::istreambuf_iterator<char> (in), std::istreambuf_iterator<char> ());
}

MethodCategories LanguageSupport::__instanceMethods__()
{
    return MethodCategories{
        {"parsing", {
            makeMethodBinding("parseSourceString:named:", &LanguageSupport::parseSourceStringNamed),
            makeMethodBinding("parseFileNamed:", &LanguageSupport::parseFileNamed),
        }},
    };
}

ASTNodePtr LanguageSupport::parseSourceStringNamed(const std::string &sourceString, const std::string &sourceStringName) const
{
    (void)sourceString;
    (void)sourceStringName;
    throw SubclassResponsibility();
}

ASTNodePtr LanguageSupport::parseFileNamed(const std::string &fileName) const
{
    return parseSourceStringNamed(readContentFromFileNamed(fileName), fileName);
}

AnyValuePtr LanguageSupport::evaluateSourceStringNamed(const std::string &sourceString, const std::string &sourceStringName) const
{
    auto ast = parseSourceStringNamed(sourceString, sourceStringName);
    return ast;
}

AnyValuePtr LanguageSupport::evaluateFileNamed(const std::string &fileName) const
{
    auto ast = parseFileNamed(fileName);
    return ast;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius