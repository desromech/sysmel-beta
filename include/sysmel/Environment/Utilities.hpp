#ifndef SYSMEL_ENVIRONMENT_UTILITIES_HPP
#define SYSMEL_ENVIRONMENT_UTILITIES_HPP
#pragma once

namespace Sysmel
{
namespace Environment
{

template<typename T>
T alignedTo(T offset, T alignment)
{
    return (offset + alignment - 1) & T(~alignment + 1);
}

template<typename T>
T floorAlignedTo(T offset, T alignment)
{
    return offset & T(~alignment + 1);
}

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_STRING_UTILITIES_HPP