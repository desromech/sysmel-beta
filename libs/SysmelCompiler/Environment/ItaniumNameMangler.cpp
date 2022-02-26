#include "Environment/ItaniumNameMangler.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/PrimitiveCharacterType.hpp"
#include "Environment/PrimitiveBooleanType.hpp"
#include "Environment/PrimitiveIntegerType.hpp"
#include "Environment/PrimitiveFloatType.hpp"

#include "Environment/Type.hpp"
#include "Environment/TypeVisitor.hpp"
#include "Environment/DecoratedType.hpp"
#include "Environment/PointerType.hpp"
#include "Environment/ReferenceType.hpp"
#include "Environment/TemporaryReferenceType.hpp"
#include "Environment/ArrayType.hpp"
#include "Environment/TupleType.hpp"
#include "Environment/VariantType.hpp"
#include "Environment/FunctionalType.hpp"
#include "Environment/PrimitiveVectorType.hpp"
#include "Environment/VirtualTable.hpp"

#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/SSAProgramEntity.hpp"
#include "Environment/SSATemplateInstance.hpp"
#include "Environment/SSAFunction.hpp"
#include "Environment/SSACodeRegion.hpp"
#include "Environment/SSACodeRegionArgument.hpp"
#include "Environment/StringUtilities.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
{

typedef std::variant<std::string, AnyValuePtrList> NameComponent;

static BootstrapTypeRegistration<ItaniumNameMangler> ItaniumNameManglerTypeRegistration;
typedef std::vector<NameComponent> NameComponents;

class ItaniumNameManglerContext
{
public:
    class ItaniumManglerTypeVisitor: public TypeVisitor
    {
    public:
        virtual AnyValuePtr visitAnyType(const TypePtr &type) override
        {
            result = context->mangleProgramEntityName(type);
            return nullptr;
        }

        virtual AnyValuePtr visitDecoratedType(const DecoratedTypePtr &type) override
        {
            if(type->isParamsDecoratedType())
                result += "U6params";
            if(type->isRestrictDecoratedType())
                result.push_back('r');
            if(type->isVolatileDecoratedType())
                result.push_back('V');
            if(type->isConstDecoratedType())
                result.push_back('K');
            result += context->mangleType(type->getBaseType());
            return nullptr;
        }

        virtual AnyValuePtr visitPointerType(const PointerTypePtr &type) override
        {
            result.push_back('P');
            result += context->mangleType(type->getBaseType());
            return nullptr;
        }

        virtual AnyValuePtr visitReferenceType(const ReferenceTypePtr &type) override
        {
            result.push_back('R');
            result += context->mangleType(type->getBaseType());
            return nullptr;
        }

        virtual AnyValuePtr visitTemporaryReferenceType(const TemporaryReferenceTypePtr &type) override
        {
            result.push_back('O');
            result += context->mangleType(type->getBaseType());
            return nullptr;
        }

        virtual AnyValuePtr visitArrayType(const ArrayTypePtr &type) override
        {
            result.push_back('A');
            if(type->size > 0)
                result += castToString(type->size);
            result.push_back('_');

            result += context->mangleType(type->elementType);
            return nullptr;
        }

        virtual AnyValuePtr visitPrimitiveVectorType(const PrimitiveVectorTypePtr &type) override
        {
            std::ostringstream out;
            if(!validAnyValue(type->getName())->isUndefined())
            {
                auto name = type->getValidNameString();
                out << 'u';
                out << name.size();
                out << name;
            }
            else
            {
                out << "15primitiveVectorI";
                out << context->mangleType(type->elementType);
                out << "Li" << type->elements << 'E';
                out << 'E';
            }
            
            result += out.str();
            return nullptr;
        }

        virtual AnyValuePtr visitVariantType(const VariantTypePtr &type) override
        {
            result += "7variantI";
            for(auto &element : type->elementTypes)
                result += context->mangleType(element);
            result.push_back('E');
            return nullptr;
        }

        virtual AnyValuePtr visitTupleType(const TupleTypePtr &type) override
        {
            result += "5tupleI";
            for(auto &element : type->elementTypes)
                result += context->mangleType(element);
            result.push_back('E');
            return nullptr;
        }

        virtual AnyValuePtr visitFunctionalType(const FunctionalTypePtr &type) override
        {
            auto receiverType = type->getReceiverType();
            if(type->isClosureType())
                result += "P7closureI";
            else
                result.push_back('F');

            if(!receiverType->isVoidType())
                result += context->mangleType(receiverType);

            auto argumentCount = type->getArgumentCount();
            for(size_t i = 0; i < argumentCount; ++i)
                result += context->mangleType(type->getArgument(i));

            result.push_back('E');
            return nullptr;
        }

        std::string result;
        ItaniumNameManglerContext *context;
    };

    std::string mangleType(const TypePtr &type)
    {
        auto it = mangler->primitiveTypeMap.find(type);
        if(it != mangler->primitiveTypeMap.end())
            return it->second;

        auto visitor = basicMakeObject<ItaniumManglerTypeVisitor> ();
        visitor->context = this;
        type->acceptTypeVisitor(visitor);
        return visitor->result;
    }

    std::string mangleTypeInfo(const TypePtr &type)
    {
        return "_ZTS" + mangleType(type);
    }

    std::string mangleVirtualTable(const VirtualTablePtr &vtable)
    {
        return "_ZTV" + mangleType(vtable->ownerType.lock());
    }

    NameComponents extractProgramEntityNameComponents(const SSAProgramEntityPtr &programEntity)
    {
        NameComponents components;
        extractProgramEntityNameComponentsOnto(programEntity, programEntity, components);
        return components;
    }

    bool isOmmittedProgramEntityFromNameComponents(const SSAProgramEntityPtr &programEntity)
    {
        if(programEntity->isSSANamespace())
        {
            auto parent = programEntity->getParent();
            return validAnyValue(parent)->isUndefined() || parent->isSSAModule();
        }

        return false;
    }

    NameComponent extractProgramEntityNameComponent(const SSAProgramEntityPtr &programEntity)
    {
        if(programEntity->isSSATemplateInstance())
            return programEntity.staticAs<SSATemplateInstance>()->getArguments();

        return programEntity->getValidNameString();
    }

    void extractProgramEntityNameComponentsOnto(const SSAProgramEntityPtr &originalProgramEntity, const SSAProgramEntityPtr &programEntity, NameComponents &destination)
    {
        auto parent = programEntity->getParent();
        if(parent)
        {
            auto mainTemplateInstanceChild = parent->getMainTemplateInstanceChild();
            if(mainTemplateInstanceChild == programEntity)
                return extractProgramEntityNameComponentsOnto(originalProgramEntity, parent, destination);
            extractProgramEntityNameComponentsOnto(originalProgramEntity, parent, destination);
        }
        else
        {
            if(originalProgramEntity->getExternalLanguageMode() == ExternalLanguageMode::None)
                destination.push_back("Sysmel");
        }

        if(!isOmmittedProgramEntityFromNameComponents(programEntity))
            destination.push_back(extractProgramEntityNameComponent(programEntity));
    }

    std::string mangleTemplateArgument(const AnyValuePtr &argument)
    {
        if(validAnyValue(argument)->isUndefined())
            return "LDnE";

        if(argument->isType())
            return mangleType(staticObjectCast<Type> (argument));

        else if(argument->isLiteralInteger())
        {
            std::ostringstream out;
            out << "Lu14LiteralInteger" << argument->asString() << "E";
            return out.str();
        }
        else if(argument->isLiteralFloat())
        {
            std::ostringstream out;
            out << "Lu12LiteralFloat" << argument->asString() << "E";
            return out.str();
            
        }
        else if(argument->isLiteralBoolean())
        {
            std::ostringstream out;
            out << "Lu14LiteralBoolean" << argument->asString() << "E";
            return out.str();
            
        }

        sysmelAssert("TODO: not yet supported" && false);
    }

    std::string encodeNameComponents(const NameComponents &components, bool mustBeNested = false, std::string prefixes = std::string())
    {
        std::ostringstream out;
        sysmelAssert(!components.empty());

        if(components.size() != 1 || mustBeNested)
        {
            out << 'N';
        }

        out << prefixes;

        for(auto &comp : components)
        {
            std::visit([&](auto && arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, std::string>)
                {
                    out << arg.size() << arg;
                }
                else
                {
                    out << 'I';
                    for(auto &templateArgument : arg)
                        out << mangleTemplateArgument(templateArgument);
                    out << 'E';
                }
            }, comp);
        }

        if(components.size() != 1 || mustBeNested)
        {
            out << 'E';
        }

        return out.str();
    }

    std::string mangleSSAProgramEntityPrefixes(const SSAProgramEntityPtr &programEntity)
    {
        std::string result;
        if(programEntity->isSSAFunction())
        {
            auto function = programEntity.staticAs<SSAFunction> ();
            auto mainCodeRegion = function->getMainCodeRegion();

            size_t firstArgumentIndex = 0;

            // Skip the implicit result type.
            if(mainCodeRegion->isReturningByReference())
                ++firstArgumentIndex;

            // Skip the receiver.
            if(firstArgumentIndex < mainCodeRegion->getArgumentCount())
            {
                const auto &firstArg = mainCodeRegion->getArgument(firstArgumentIndex);
                if(firstArg->isReceiver())
                {
                    auto receiverType = firstArg->getValueType();
                    if(receiverType->isConstOrConstReferenceType())
                        result.push_back('K');
                }
            }
        }

        return result;
    }

    std::string mangleSSAProgramEntitySuffixes(const SSAProgramEntityPtr &programEntity)
    {
        std::string result;
        if(programEntity->isSSAFunction())
        {
            auto function = programEntity.staticAs<SSAFunction> ();
            auto mainCodeRegion = function->getMainCodeRegion();

            size_t firstArgumentIndex = 0;

            // Skip the implicit result type.
            if(mainCodeRegion->isReturningByReference())
                ++firstArgumentIndex;

            // Skip the receiver.
            if(firstArgumentIndex < mainCodeRegion->getArgumentCount())
            {
                const auto &firstArg = mainCodeRegion->getArgument(firstArgumentIndex);
                if(firstArg->isReceiver())
                    ++firstArgumentIndex;
            }

            // Append each one of the arguments.
            for(size_t i = firstArgumentIndex; i < mainCodeRegion->getArgumentCount(); ++i)
                result += mangleType(mainCodeRegion->getArgument(i)->getSourceType());

            if(firstArgumentIndex == mainCodeRegion->getArgumentCount())
                result.push_back('v');
        }

        return result;
    }

    std::string mangleSSAProgramEntityName(const SSAProgramEntityPtr &programEntity)
    {
        return encodeNameComponents(extractProgramEntityNameComponents(programEntity), false, mangleSSAProgramEntityPrefixes(programEntity)) + mangleSSAProgramEntitySuffixes(programEntity);
    }

    std::string mangleSSAProgramEntity(const SSAProgramEntityPtr &programEntity)
    {
        if(programEntity->getExternalLanguageMode() == ExternalLanguageMode::C)
            return programEntity->getValidNameString();

        return "_Z" + mangleSSAProgramEntityName(programEntity);
    }

    std::string mangleProgramEntityName(const ProgramEntityPtr &programEntity)
    {
        auto ssaValue = programEntity->asSSAValueRequiredInPosition(ASTSourcePosition::empty());
        sysmelAssert(ssaValue->isSSAProgramEntity());

        return mangleSSAProgramEntityName(staticObjectCast<SSAProgramEntity> (ssaValue));
    }

    std::string mangleProgramEntity(const ProgramEntityPtr &programEntity)
    {
        auto ssaValue = programEntity->asSSAValueRequiredInPosition(ASTSourcePosition::empty());
        sysmelAssert(ssaValue->isSSAProgramEntity());

        return mangleSSAProgramEntity(staticObjectCast<SSAProgramEntity> (ssaValue));
    }

    ItaniumNameMangler *mangler = nullptr;
};

ItaniumNameMangler::ItaniumNameMangler()
{
    primitiveTypeMap = {
        {Type::getVoidType(), "v"},
        {Type::getUndefinedType(), "Dn"},

        {Boolean8::__staticType__(), "b"},

        {UInt8::__staticType__(), "h"},
        {UInt16::__staticType__(), "t"},
        {UInt32::__staticType__(), "j"},
        {UInt64::__staticType__(), "y"},

        {Int8::__staticType__(), "a"},
        {Int16::__staticType__(), "s"},
        {Int32::__staticType__(), "i"},
        {Int64::__staticType__(), "x"},

        {Char8::__staticType__(), "Du"},
        {Char16::__staticType__(), "Ds"},
        {Char32::__staticType__(), "Di"},

        {Float16::__staticType__(), "Dh"},
        {Float32::__staticType__(), "f"},
        {Float64::__staticType__(), "d"},

        {Type::getCVarArgsType(), "z"},
    };
}

std::string ItaniumNameMangler::mangleType(const TypePtr &type)
{
    ItaniumNameManglerContext context;
    context.mangler = this;
    return context.mangleType(type);
}

std::string ItaniumNameMangler::mangleTypeInfo(const TypePtr &type)
{
    ItaniumNameManglerContext context;
    context.mangler = this;
    return context.mangleTypeInfo(type);
}

std::string ItaniumNameMangler::mangleVirtualTable(const VirtualTablePtr &vtable)
{
    ItaniumNameManglerContext context;
    context.mangler = this;
    return context.mangleVirtualTable(vtable);
}

std::string ItaniumNameMangler::mangleSSAProgramEntity(const SSAProgramEntityPtr &programEntity)
{
    ItaniumNameManglerContext context;
    context.mangler = this;
    return context.mangleSSAProgramEntity(programEntity);
}

std::string ItaniumNameMangler::mangleProgramEntity(const ProgramEntityPtr &programEntity)
{
    ItaniumNameManglerContext context;
    context.mangler = this;
    return context.mangleProgramEntity(programEntity);
}

} // End of namespace Environment
} // End of namespace Sysmel