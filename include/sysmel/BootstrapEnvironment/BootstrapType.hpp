#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BOOTSTRAP_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BOOTSTRAP_TYPE_HPP
#pragma once

#include "Type.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am the base interface for a type that is specifically defined by the bootstrap environment.
 */
class BootstrapType : public SubtypeOf<Type, BootstrapType>
{
public:
    static constexpr char const __typeName__[] = "BootstrapType";

    virtual bool isBootstrapType() const override;
    virtual bool supportsDynamicCompileTimeMessageSend() const override;
    virtual bool supportsMessageAnalysisByLiteralValueReceivers() const override;

    void initializeWithMetadata(const StaticBootstrapDefinedTypeMetadata *theStaticMetadata);

    std::string printString() const override;

    virtual TypePtr getType() const override;
    void setType(const TypePtr &theMetaType);

protected:
    const StaticBootstrapDefinedTypeMetadata *staticMetadata;
    TypePtr metaType;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BOOTSTRAP_TYPE_HPP