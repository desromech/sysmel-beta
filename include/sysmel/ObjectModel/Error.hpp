#ifndef SYSMEL_OBJECT_MODEL_ERROR_HPP
#define SYSMEL_OBJECT_MODEL_ERROR_HPP
#pragma once

#include <stdexcept>

namespace SysmelMoebius
{
namespace ObjectModel
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

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_OBJECT_MODEL_ERROR_HPP
