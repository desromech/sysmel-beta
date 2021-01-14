#include "sysmel/Compiler/Sysmel/Parser.hpp"
#include "sysmel/Compiler/Sysmel/Visitors.hpp"
#include "sysmel/Compiler/Sysmel/BootstrapInterpreter.hpp"
#include <fstream>
#include <iostream>

using namespace SysmelMoebius;

bool evaluateBootstrapStreamNamed(std::istream &in, const std::string &fileName, const ObjectModel::ObjectEnvironmentPtr &environment)
{
    auto content = std::string(std::istreambuf_iterator<char> (in), std::istreambuf_iterator<char> ());

    using namespace Compiler::Sysmel;
    auto ast = parseString(content, fileName);
    validateASTParseErrors(ast, [&](ASTParseErrorNode &parseErrorNode) {
        std::cout << parseErrorNode.sourcePosition << ": " << parseErrorNode.errorMessage << '\n';
        std::cout << parseErrorNode.sourcePosition.content() << std::endl;
    });

    auto result = evaluateBootstrapScriptAST(ast, createDefaultBootstrapEvaluationEnvironmentFor(environment));
    std::cout << result->asString() << std::endl;

    return true;
}

bool evaluateBootstrapFileNamed(const std::string &fileName, const ObjectModel::ObjectEnvironmentPtr &environment)
{
    if(fileName == "-")
    {
        return evaluateBootstrapStreamNamed(std::cin, fileName, environment);
    }

    std::ifstream in(fileName);
    if(!in.good())
        return false;

    return evaluateBootstrapStreamNamed(in, fileName, environment);
}

int main(int argc, const char *argv[])
{
    auto environment = std::make_shared<ObjectModel::StandardObjectEnvironment> ();

    for(int i = 1; i < argc; ++i)
    {
        evaluateBootstrapFileNamed(argv[i], environment);
    }

    return 0;
}
