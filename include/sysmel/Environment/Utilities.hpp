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

inline uint32_t asHighPowerOfTwo(uint32_t x)
{
    if(x == 0) 
        return 1;

    //https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
    auto y = x - 1;
    y |= y >> 1;
    y |= y >> 2;
    y |= y >> 4;
    y |= y >> 8;
    y |= y >> 16;
    return y + 1;
}

inline uint64_t asHighPowerOfTwo(uint64_t x)
{
    if(x == 0) 
        return 1;

    //https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
    auto y = x - 1;
    y |= y >> 1;
    y |= y >> 2;
    y |= y >> 4;
    y |= y >> 8;
    y |= y >> 16;
    y |= y >> 32;
    return y + 1;
}

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_STRING_UTILITIES_HPP