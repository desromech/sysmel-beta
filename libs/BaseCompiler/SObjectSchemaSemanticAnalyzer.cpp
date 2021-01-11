#include "sysmel/BaseCompiler/SObjectSchema.hpp"
#include <unordered_set>
#include <iostream>

namespace SysmelMoebius
{
namespace Compiler
{
namespace SObjectSchema
{

struct SchemaCTypeEvaluator : SchemaEntityVisitor
{
    SchemaCTypeEvaluator(const SchemaPtr &cschema, const SchemaEntityPtr &cselfType)
        : schema(cschema), selfType(cselfType) {}

    const SchemaPtr &schema;
    SchemaEntityPtr selfType;

    virtual std::any visitType(SchemaType &) { abort(); }
    virtual std::any visitMethod(SchemaMethod &) { abort(); }

    virtual std::any visitSelfTypeExpression(SchemaMethodSelfTypeExpression &)
    {
        return selfType;
    }

    virtual std::any visitTypeToCTypeNameExpression(SchemaTypeToCTypeNameExpression &e)
    {
        return std::any_cast<SchemaEntityPtr> (visitEntity(*e.typeExpression))->name;
    }

    virtual std::any visitVoidLiteralValue(SchemaVoidLiteralValue &) { abort(); }
    virtual std::any visitNilLiteralValue(SchemaNilLiteralValue &) { abort(); }
    virtual std::any visitFalseLiteralValue(SchemaFalseLiteralValue &) { abort(); }
    virtual std::any visitTrueLiteralValue(SchemaTrueLiteralValue &) { abort(); }
    virtual std::any visitIntegerLiteralValue(SchemaIntegerLiteralValue &) { abort(); }
    virtual std::any visitFloatLiteralValue(SchemaFloatLiteralValue &) { abort(); }
    virtual std::any visitCharacterLiteralValue(SchemaCharacterLiteralValue &) { abort(); }
    virtual std::any visitStringLiteralValue(SchemaStringLiteralValue &) { abort(); }
    virtual std::any visitSymbolLiteralValue(SchemaSymbolLiteralValue &v) { return v.value.value; }
};

struct SchemaTypeSolver : SchemaEntityVisitor
{
    SchemaTypeSolver(const SchemaPtr &cschema)
        : schema(cschema) {}

    const SchemaPtr &schema;

    SchemaType *selfType = nullptr;
    SchemaMethod *currentMethod = nullptr;

    virtual std::any visitType(SchemaType &entity) override
    {
        selfType = &entity;

        // Visit the methods.
        for(auto &[key, value] : entity.methodDictionary)
        {
            visitEntity(*value);
        }

        // Copy the instance type dependent methods.
        if(entity.superType)
        {
            for (auto &method : entity.superType->instanceDependentMethods)
            {
                if(entity.lookupSelector(method->selector) == method)
                {
                    entity.methodDictionary[method->selector] = method;
                    entity.instanceDependentMethods.push_back(method);
                }
            }
        }

        // Solve the CType.
        {
            auto ctypeMethod = entity.lookupSelector("ctype");
            if(!ctypeMethod)
                throw std::runtime_error("Required #ctype method is missing in type " + entity.name + ".");

            entity.ctype = std::any_cast<std::string> (
                SchemaCTypeEvaluator(schema, std::static_pointer_cast<SchemaType> (entity.shared_from_this()))
                    .visitEntity(*ctypeMethod->valueExpression)
            );
        }

        return std::any();
    }

    virtual std::any visitMethod(SchemaMethod &method) override
    {
        currentMethod = &method;

        auto returnType = std::any_cast<SchemaTypePtr> (visitEntity(*method.valueExpression));
        method.returnType = returnType;
        if(selfType->superType)
        {
            auto superMethod = selfType->superType->lookupSelector(method.selector);
            if(superMethod)
            {
                method.isOverride = true;
                auto superMethodReturnType = superMethod->returnType.lock();
                if(superMethodReturnType != returnType)
                    throw std::runtime_error("Overriden method [" + selfType->name + "]>>#" + method.selector +
                    " has an incompatible return type with the super definition. " + returnType->name + " vs. " + superMethodReturnType->name + ".");
            }
        }

        if(method.valueExpression->isInstanceTypeDependentExpression())
            selfType->instanceDependentMethods.push_back(std::static_pointer_cast<SchemaMethod> (method.shared_from_this()));

        return std::any();
    }

    virtual std::any visitSelfTypeExpression(SchemaMethodSelfTypeExpression &) override
    {
        return schema->getRequiredTypeNamed("Type");
    }

    virtual std::any visitTypeToCTypeNameExpression(SchemaTypeToCTypeNameExpression &) override
    {
        return schema->getRequiredTypeNamed("LiteralSymbolType");
    }

    virtual std::any visitVoidLiteralValue(SchemaVoidLiteralValue &) override
    {
        return schema->getRequiredTypeNamed("Void");
    }

    virtual std::any visitNilLiteralValue(SchemaNilLiteralValue &) override
    {
        return schema->getRequiredTypeNamed("UndefinedType");
    }

    virtual std::any visitFalseLiteralValue(SchemaFalseLiteralValue &) override
    {
        return schema->getRequiredTypeNamed("LiteralBooleanType");
    }

    virtual std::any visitTrueLiteralValue(SchemaTrueLiteralValue &) override
    {
        return schema->getRequiredTypeNamed("LiteralBooleanType");
    }

    virtual std::any visitIntegerLiteralValue(SchemaIntegerLiteralValue &) override
    {
        return schema->getRequiredTypeNamed("LiteralIntegerType");
    }

    virtual std::any visitFloatLiteralValue(SchemaFloatLiteralValue &) override
    {
        return schema->getRequiredTypeNamed("LiteralFloatType");
    }

    virtual std::any visitCharacterLiteralValue(SchemaCharacterLiteralValue &) override
    {
        return schema->getRequiredTypeNamed("LiteralCharacterType");
    }

    virtual std::any visitStringLiteralValue(SchemaStringLiteralValue &) override
    {
        return schema->getRequiredTypeNamed("LiteralStringType");
    }

    virtual std::any visitSymbolLiteralValue(SchemaSymbolLiteralValue &) override
    {
        return schema->getRequiredTypeNamed("LiteralSymbolType");
    }

    void processSchema()
    {
        // Solve the types.
        for(auto &entity : schema->sortedTypes)
        {
            visitEntity(*entity);
        }
    }

};

static void solveSuperType(const SchemaPtr &schema, const SchemaTypePtr &type, std::unordered_set<SchemaType*> &visitedSet)
{
    if(visitedSet.find(type.get()) != visitedSet.end())
        return;
    visitedSet.insert(type.get());

    if(type->superTypeName.empty())
    {
        schema->sortedTypes.push_back(type);
        return;
    }

    auto it = schema->entityNameDictionary.find(type->superTypeName);
    if(it == schema->entityNameDictionary.end())
        throw std::runtime_error("Failed to solve super type name " + type->superTypeName);

    if(!it->second->isType())
        throw std::runtime_error("Super type entity " + type->superTypeName + " is not a type.");

    type->superType = std::static_pointer_cast<SchemaType> (it->second);
    solveSuperType(schema, type->superType, visitedSet);
    schema->sortedTypes.push_back(type);
}

static void solveSuperTypes(const SchemaPtr &schema)
{
    std::unordered_set<SchemaType*> visitedSet;
    for(auto &entity : schema->entities)
    {
        if(!entity->isType())
            continue;

        auto type = std::static_pointer_cast<SchemaType> (entity);
        solveSuperType(schema, type, visitedSet);
    }
}

void semanticAnalyzeSchema(const SchemaPtr &schema)
{
    // Solve the super types.
    solveSuperTypes(schema);

    SchemaTypeSolver{schema}.processSchema();
}

} // End of namespace SObjectSchema
} // End of namespace Compiler
} // End of namespace SysmelMoebius
