#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_EXCEPTION_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_EXCEPTION_HPP
#pragma once

#include "CompilerObject.hpp"
#include <stdexcept>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(Exception);

/**
 * I am the base interface for a Smalltalk style exception that is exposed in the bootstrap environment.
 */
class Exception : public SubtypeOf<CompilerObject, Exception>
{
public:
    static constexpr char const __typeName__[] = "Exception";

    [[noreturn]] virtual void signal();
    [[noreturn]] virtual void signalWithMessage(const std::string &message);

    virtual const std::string &getMessageText() const;
    virtual void setMessageText(const std::string &message);

    virtual std::string getDescription() const;

protected:
    std::string messageText;
};

template<typename T>
[[noreturn]] void signalNew()
{
    std::make_shared<T> ()->signal();
    abort();
}

template<typename T>
[[noreturn]] void signalNewWithMessage(const std::string &message)
{
    std::make_shared<T> ()->signalWithMessage(message);
    abort();
}

class ExceptionWrapper : public std::exception
{
public:
    ExceptionWrapper(const ExceptionPtr &theException);

    virtual const char* what() const noexcept override;

    const ExceptionPtr &getException() const;

protected:
    ExceptionPtr exception;

    mutable bool hasRetrievedDescription = false;
    mutable std::string description;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_EXCEPTION_HPP