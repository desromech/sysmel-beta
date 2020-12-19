#include "sysmel/Compiler/Sysmel/Parser.hpp"
#include "sysmel/Compiler/Sysmel/Visitors.hpp"
#include <fstream>
#include <iostream>

void lintFileNamed(const std::string &fileName)
{
    std::ifstream in(fileName);
    if(!in.good())
        return;

    auto content = std::string(std::istreambuf_iterator<char> (in), std::istreambuf_iterator<char> ());

    using namespace SysmelMoebius::Compiler::Sysmel;
    auto ast = parseString(content, fileName);
    validateASTParseErrors(ast, [&](ASTParseErrorNode &parseErrorNode) {
        std::cout << parseErrorNode.sourcePosition << ": " << parseErrorNode.errorMessage << '\n';
        std::cout << parseErrorNode.sourcePosition.content() << std::endl;
    });
}

int main(int argc, const char *argv[])
{
    for(int i = 1; i < argc; ++i)
    {
        lintFileNamed(argv[i]);
    }

    return 0;
}
