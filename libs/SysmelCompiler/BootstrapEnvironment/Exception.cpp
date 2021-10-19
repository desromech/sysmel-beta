#include "sysmel/BootstrapEnvironment/Exception.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<Exception> exceptionTypeRegistration;

void Exception::signal()
{
    throw ExceptionWrapper(shared_from_this());
}

void Exception::signalWithMessage(const std::string &message)
{
    messageText = message;
    signal();
    abort();
}

const std::string &Exception::getMessageText() const
{
    return messageText;
}

void Exception::setMessageText(const std::string &message)
{
    messageText = message;
}

std::string Exception::getDescription() const
{
    if(messageText.empty())
        return getType()->asString();
    return getType()->asString() + ": " + messageText;
}

ExceptionWrapper::ExceptionWrapper(const ExceptionPtr &theException)
    : exception(theException)
{
}

const char* ExceptionWrapper::what() const noexcept
{
    if(!hasRetrievedDescription)
    {
        if(!RuntimeContext::getActive())
            return "Exception propagated beyond its runtime context scope.";
        description = exception->getDescription();
        hasRetrievedDescription = true;
    }
    return description.c_str();
}

const ExceptionPtr &ExceptionWrapper::getException() const
{
    return exception;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius