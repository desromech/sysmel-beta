#ifndef SYSMEL_ENVIRONMENT_MESSAGE_CHAIN_RECEIVER_NAME_HPP
#define SYSMEL_ENVIRONMENT_MESSAGE_CHAIN_RECEIVER_NAME_HPP
#pragma once

#include "CompilerObject.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a literal symbol value.
 */
class MessageChainReceiverName : public SubtypeOf<CompilerObject, MessageChainReceiverName>
{
public:
    static constexpr char const __typeName__[] = "MessageChainReceiverName";
    
    virtual bool isHiddenNameSymbol() const override;
    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    ASTSourcePositionPtr sourcePosition;
};


} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_MESSAGE_CHAIN_RECEIVER_NAME_HPP