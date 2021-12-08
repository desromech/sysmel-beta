#include "Environment/MetaBuilderInstanceContext.hpp"
#include "Environment/ASTNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<MetaBuilderInstanceContext> metaBuilderTypeRegistration;

ASTSourcePositionPtr MetaBuilderInstanceContext::concreteSourcePosition()
{
    return instanceNode->sourcePosition;
}

} // End of namespace Environment
} // End of namespace Sysmel
