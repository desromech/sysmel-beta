#ifndef SYSMEL_ENVIRONMENT_PATTERN_MATCHING_VALUE_NAME_HPP
#define SYSMEL_ENVIRONMENT_PATTERN_MATCHING_VALUE_NAME_HPP
#pragma once

#include "CompilerObject.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am a literal symbol value.
 */
class SYSMEL_COMPILER_LIB_EXPORT PatternMatchingValueName : public SubtypeOf<CompilerObject, PatternMatchingValueName>
{
public:
    static constexpr char const __typeName__[] = "PatternMatchingValueName";
    
    virtual bool isHiddenNameSymbol() const override;
    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    ASTSourcePositionPtr sourcePosition;
};


} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_PATTERN_MATCHING_VALUE_NAME_HPP