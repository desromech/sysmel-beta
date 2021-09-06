#include "sysmel/Compiler/Sysmel/Parser.hpp"
#include "sysmel/Compiler/Sysmel/Visitors.hpp"
#include "sysmel/ObjectModel/AnyValue.hpp"
#include "sysmel/ObjectModel/LiteralInteger.hpp"
#include "sysmel/ObjectModel/LiteralFloat.hpp"
#include "sysmel/ObjectModel/LiteralString.hpp"
#include "sysmel/ObjectModel/LiteralSymbol.hpp"
#include "sysmel/ObjectModel/RuntimeContext.hpp"
#include <fstream>
#include <iostream>

static std::string readContentFromFileNamed(const std::string &fileName)
{
    if(fileName == "-")
    {
        return std::string(std::istreambuf_iterator<char> (std::cin), std::istreambuf_iterator<char> ());
    }

    std::ifstream in(fileName);
    if(!in.good())
    {
        std::cerr << "Failed to read file named: " << fileName << std::endl;
        return std::string();
    }

    return std::string(std::istreambuf_iterator<char> (in), std::istreambuf_iterator<char> ());
}

namespace TestEvaluator
{
using namespace SysmelMoebius::Compiler::Sysmel;
using namespace SysmelMoebius::ObjectModel;

class BasicASTEvaluator : public ASTVisitor
{
public:
    virtual std::any visitParseErrorNode(ASTParseErrorNode &) override
    {
        return std::any();
    }

    virtual std::any visitExpressionListNode(ASTExpressionListNode &node) override
    {
        std::any result;
        for(auto expr : node.expressions)
        {
            result = visitNode(*expr);
        }

        return result;
    }
    
    virtual std::any visitPragmaNode(ASTPragmaNode &) override
    {
        return std::any();
    }
    
    virtual std::any visitBlockNode(ASTBlockNode &) override
    {
        return std::any();
    }
    
    virtual std::any visitBlockClosureArgumentNode(ASTBlockClosureArgumentNode &)
    {
        return std::any();
    }
    virtual std::any visitBlockClosureSignatureNode(ASTBlockClosureSignatureNode &)
    {
        return std::any();
    }

    virtual std::any visitIntegerLiteralNode(ASTIntegerLiteralNode &node) override
    {
        return AnyValuePtr(LiteralInteger::makeFor(node.value));
    }

    virtual std::any visitFloatLiteralNode(ASTFloatLiteralNode &node) override
    {
        return AnyValuePtr(std::make_shared<LiteralFloat> (node.value));
    }

    virtual std::any visitCharacterLiteralNode(ASTCharacterLiteralNode &node) override
    {
        return AnyValuePtr(LiteralInteger::makeForCharacter(node.value));
    }

    virtual std::any visitStringLiteralNode(ASTStringLiteralNode &node) override
    {
        return AnyValuePtr(LiteralString::makeFor(node.value));
    }

    virtual std::any visitSymbolLiteralNode(ASTSymbolLiteralNode &node) override
    {
        return AnyValuePtr(LiteralSymbol::intern(node.value));
    }

    virtual std::any visitIdentifierReferenceNode(ASTIdentifierReferenceNode &node) override
    {
        return std::any();
    }

    virtual std::any visitMessageSendNode(ASTMessageSendNode &node) override
    {
        auto receiver = std::any_cast<AnyValuePtr> (visitNode(*node.receiver));

        auto selector = std::any_cast<AnyValuePtr> (visitNode(*node.selector));

        std::vector<AnyValuePtr> arguments;
        arguments.reserve(node.arguments.size());
        for(auto &arg : node.arguments)
        {
            auto argValue = std::any_cast<AnyValuePtr> (visitNode(*arg));
        }

        return receiver->performWithArguments(selector, arguments);
    }

    virtual std::any visitMessageChainNode(ASTMessageChainNode &node) override
    {
        return std::any();
    }

    virtual std::any visitMessageChainMessageNode(ASTMessageChainMessageNode &) override
    {
        return std::any();
    }

    virtual std::any visitCallNode(ASTCallNode &) override
    {
        return std::any();
    }

    virtual std::any visitSubscriptNode(ASTSubscriptNode &) override
    {
        return std::any();
    }

    virtual std::any visitMakeTupleNode(ASTMakeTupleNode &) override
    {
        return std::any();
    }

    virtual std::any visitMakeDictionaryNode(ASTMakeDictionaryNode &) override
    {
        return std::any();
    }

    virtual std::any visitDictionaryElementNode(ASTDictionaryElementNode &) override
    {
        return std::any();
    }

    virtual std::any visitLiteralArrayNode(ASTLiteralArrayNode &) override
    {
        return std::any();
    }

    virtual std::any visitQuoteNode(ASTQuoteNode &) override
    {
        return std::any();
    }

    virtual std::any visitQuasiQuoteNode(ASTQuasiQuoteNode &) override
    {
        return std::any();
    }

    virtual std::any visitQuasiUnquoteNode(ASTQuasiUnquoteNode &) override
    {
        return std::any();
    }

    virtual std::any visitSpliceNode(ASTSpliceNode &) override
    {
        return std::any();
    }
};

}

void evalString(const std::string &sourceString, const std::string &sourceName)
{
    using namespace SysmelMoebius::Compiler::Sysmel;
    using namespace SysmelMoebius::ObjectModel;
    auto ast = parseString(sourceString, sourceName);
    bool hasError = false;
    validateASTParseErrors(ast, [&](ASTParseErrorNode &parseErrorNode) {
        std::cout << parseErrorNode.sourcePosition << ": " << parseErrorNode.errorMessage << '\n';
        std::cout << parseErrorNode.sourcePosition.content() << std::endl;
        hasError = true;
    });

    if(hasError)
        return;

    auto result = std::any_cast<AnyValuePtr> (TestEvaluator::BasicASTEvaluator().visitNode(*ast));
    std::cout << result->printString() << std::endl;
}

void evalFileNamed(const std::string &fileName)
{
    auto content = readContentFromFileNamed(fileName);
    if(content.empty())
        return;

    evalString(content, fileName == "-" ? "stdin" : fileName);
}

int main(int argc, const char *argv[])
{
    SysmelMoebius::ObjectModel::RuntimeContext::create()->activeDuring([&](){
        for(int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if(arg == "-eval")
            {
                evalString(argv[++i], "<command line arg>");
            }
            else
            {
                evalFileNamed(argv[i]);
            }
        }
    });

    return 0;
}
