#ifndef SYSMEL_ENVIRONMENT_EXCEPTION_HPP
#define SYSMEL_ENVIRONMENT_EXCEPTION_HPP
#pragma once

#include "CompilerObject.hpp"
#include <stdexcept>

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(Exception);

/**
 * I am the base interface for a Smalltalk style exception that is exposed in the bootstrap environment.
 */
class SYSMEL_COMPILER_LIB_EXPORT Exception : public SubtypeOf<CompilerObject, Exception>
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
    basicMakeObject<T> ()->signal();
    abort();
}

template<typename T>
[[noreturn]] void signalNewWithMessage(const std::string &message)
{
    basicMakeObject<T> ()->signalWithMessage(message);
    abort();
}

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4275 )
#endif

class SYSMEL_COMPILER_LIB_EXPORT ExceptionWrapper : public std::exception
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

#ifdef _MSC_VER
#pragma warning( pop )
#endif

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_EXCEPTION_HPP