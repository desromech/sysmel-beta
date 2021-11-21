#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_MESSAGE_CHAIN_RECEIVER_NAME_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_MESSAGE_CHAIN_RECEIVER_NAME_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a literal symbol value.
 */
class MessageChainReceiverName : public SubtypeOf<CompilerObject, MessageChainReceiverName>
{
public:
    static constexpr char const __typeName__[] = "MessageChainReceiverName";
    
    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    ASTSourcePositionPtr sourcePosition;
};


} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_MESSAGE_CHAIN_RECEIVER_NAME_HPP