#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_CONTROL_FLOW_UTILITIES_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_CONTROL_FLOW_UTILITIES_HPP
#pragma once

#include "Exception.hpp"

namespace Sysmel
{
namespace Environment
{

template<typename DT, typename ET>
auto doWithEnsure(const DT &doBlock, const ET &ensureBlock)
{
    struct RunEnsure
    {
        RunEnsure(const ET &initEnsureBlock)
            : ensureBlock(initEnsureBlock)
        {
            // Nothing required here.
        }

        ~RunEnsure()
        {
            ensureBlock();
        }
        const ET &ensureBlock;
    } RunEnsure(ensureBlock);

    return doBlock();
}

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_CONTROL_FLOW_UTILITIES_HPP
