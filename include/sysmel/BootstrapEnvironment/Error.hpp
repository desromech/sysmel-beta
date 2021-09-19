#ifndef SYSMEL_OBJECT_MODEL_ERROR_HPP
#define SYSMEL_OBJECT_MODEL_ERROR_HPP
#pragma once

#include <stdexcept>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

class Error : public std::runtime_error
{
public:
    Error(const std::string &message)
        : std::runtime_error(message) {}
};

class CannotEvaluateMessage : public Error
{
public:
    CannotEvaluateMessage(const std::string &message = "This object cannot evaluate messages.")
        : Error(message) {}
};

class MessageNotUnderstood : public Error
{
public:
    MessageNotUnderstood(const std::string &message = "Message not understood.")
        : Error(message) {}
};

class ArithmeticError : public Error
{
public:
    ArithmeticError(const std::string &message = "Arithmetic error.")
        : Error(message) {}
};

class DivisionByZeroError : public ArithmeticError
{
public:
    DivisionByZeroError(const std::string &message = "Division by zero is an undefined operation.")
        : ArithmeticError(message) {}
};

class DomainError : public ArithmeticError
{
public:
    DomainError(const std::string &message = "The supplied arguments are outside of the function domain.")
        : ArithmeticError(message) {}
};

class CannotUnwrap : public Error
{
public:
    CannotUnwrap(const std::string &message = "Cannot unwrap the object as the specified type.")
        : Error(message) {}
};

class ArgumentsCountMismatch : public Error
{
public:
    ArgumentsCountMismatch(size_t initialExpectedCount, size_t initialCallCount, const std::string &message = "Argument count mismatch")
        : Error(message), expectedCount(initialExpectedCount), callCount(initialCallCount) {}

protected:
    size_t expectedCount;
    size_t callCount;
};

class CannotOverloadPatternMatchingMethod : public Error
{
public:
    CannotOverloadPatternMatchingMethod(const std::string &message = "Cannot overload the existent method with additional patterns.")
        : Error(message) {}
};

class NotMatchingPatternFound : public Error
{
public:
    NotMatchingPatternFound(const std::string &message = "Failed to find matching pattern.")
        : Error(message) {}
};

class AmbiguousMatchingPatternsFound : public Error
{
public:
    AmbiguousMatchingPatternsFound(const std::string &message = "Ambiguous matching patterns have been found.")
        : Error(message) {}
};

class SubclassResponsibility : public Error
{
public:
    SubclassResponsibility(const std::string &message = "This method should be implemented in a subclass.")
        : Error(message) {}
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_OBJECT_MODEL_ERROR_HPP
