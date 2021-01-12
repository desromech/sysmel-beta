#include "sysmel/ObjectModel/SObjectSchema.hpp"
#include "UnitTest++/UnitTest++.h"

using namespace SysmelMoebius::ObjectModel;


SUITE(SObjectSchema)
{
    TEST(Empty)
    {
        auto emptySchema = std::make_shared<Schema> ();
        CHECK(emptySchema->getTypeDefinitions().empty());
        CHECK(!emptySchema->getTypeDefinitionNamed("Unexistent"));

        auto serializedData = emptySchema->serializeToBinaryData();
        CHECK_EQUAL(sizeof(SchemaDefinitionHeader), serializedData.size());

        auto deserializedSchema = Schema::deserializeFromBinaryData(serializedData);
        CHECK(deserializedSchema);
        CHECK(deserializedSchema->getTypeDefinitions().empty());
        CHECK(!deserializedSchema->getTypeDefinitionNamed("Unexistent"));
    }

    TEST(BasicType)
    {
        auto schema = std::make_shared<Schema> ();

        auto float64Type = schema->createBasicType("Float64", SchemaTypeLayout::Float, 8, 4);
        CHECK_EQUAL(SchemaTypeLayout::Float, float64Type->layout);
        CHECK_EQUAL(8u, float64Type->instanceSize);
        CHECK_EQUAL(4u, float64Type->instanceAlignment);

        CHECK(!schema->getTypeDefinitions().empty());
        CHECK_EQUAL(float64Type, schema->getTypeDefinitionNamed("Float64"));

        auto serializedData = schema->serializeToBinaryData();
        auto deserializedSchema = Schema::deserializeFromBinaryData(serializedData);
        CHECK(deserializedSchema);
        CHECK(!deserializedSchema->getTypeDefinitions().empty());

        auto readFloat64Type = deserializedSchema->getTypeDefinitionNamed("Float64");
        CHECK_EQUAL(SchemaTypeLayout::Float, readFloat64Type->layout);
        CHECK_EQUAL(8u, readFloat64Type->instanceSize);
        CHECK_EQUAL(4u, readFloat64Type->instanceAlignment);
    }

    TEST(DerivedType)
    {
        auto schema = std::make_shared<Schema> ();

        auto uint64Type = schema->createBasicType("UInt64", SchemaTypeLayout::UnsignedInteger, 8, 8);
        CHECK_EQUAL(SchemaTypeLayout::UnsignedInteger, uint64Type->layout);
        CHECK_EQUAL(8u, uint64Type->instanceSize);
        CHECK_EQUAL(8u, uint64Type->instanceAlignment);

        auto uintPointerType = schema->createAliasType("UIntPointer", uint64Type);
        CHECK_EQUAL(SchemaTypeLayout::Alias, uintPointerType->layout);
        CHECK_EQUAL(uint64Type, uintPointerType->baseType.lock());
        CHECK_EQUAL(0u, uintPointerType->instanceSize);
        CHECK_EQUAL(0u, uintPointerType->instanceAlignment);

        CHECK(!schema->getTypeDefinitions().empty());
        CHECK_EQUAL(uint64Type, schema->getTypeDefinitionNamed("UInt64"));

        auto serializedData = schema->serializeToBinaryData();
        auto deserializedSchema = Schema::deserializeFromBinaryData(serializedData);
        CHECK(deserializedSchema);
        CHECK(!deserializedSchema->getTypeDefinitions().empty());

        auto readUInt64Type = deserializedSchema->getTypeDefinitionNamed("UInt64");
        CHECK_EQUAL(SchemaTypeLayout::UnsignedInteger, readUInt64Type->layout);
        CHECK_EQUAL(8u, readUInt64Type->instanceSize);
        CHECK_EQUAL(8u, readUInt64Type->instanceAlignment);

        auto readUIntPointerType = deserializedSchema->getTypeDefinitionNamed("UIntPointer");
        CHECK_EQUAL(SchemaTypeLayout::Alias, readUIntPointerType->layout);
        CHECK_EQUAL(readUInt64Type, readUIntPointerType->baseType.lock());
        CHECK_EQUAL(0u, readUIntPointerType->instanceSize);
        CHECK_EQUAL(0u, readUIntPointerType->instanceAlignment);
    }

    TEST(StructureType)
    {
        auto schema = std::make_shared<Schema> ();

        auto uint8Type = schema->createBasicType("UInt8", SchemaTypeLayout::UnsignedInteger, 1, 1);
        CHECK_EQUAL(SchemaTypeLayout::UnsignedInteger, uint8Type->layout);
        CHECK_EQUAL(1u, uint8Type->instanceSize);
        CHECK_EQUAL(1u, uint8Type->instanceAlignment);

        auto uint64Type = schema->createBasicType("UInt64", SchemaTypeLayout::UnsignedInteger, 8, 8);
        CHECK_EQUAL(SchemaTypeLayout::UnsignedInteger, uint64Type->layout);
        CHECK_EQUAL(8u, uint64Type->instanceSize);
        CHECK_EQUAL(8u, uint64Type->instanceAlignment);

        auto structureType = schema->createStructureType("TestStruct");
        structureType->createSlot("byte", uint8Type);
        structureType->createSlot("qword", uint64Type);
        structureType->computeSlotLayout();
        CHECK_EQUAL(SchemaTypeLayout::Structure, structureType->layout);
        CHECK_EQUAL(16u, structureType->instanceSize);
        CHECK_EQUAL(8u, structureType->instanceAlignment);
        CHECK_EQUAL(uint8Type, structureType->getSlotNamed("byte")->type.lock());
        CHECK_EQUAL(0u, structureType->getSlotNamed("byte")->offset);
        CHECK_EQUAL(uint64Type, structureType->getSlotNamed("qword")->type.lock());
        CHECK_EQUAL(8u, structureType->getSlotNamed("qword")->offset);

        CHECK(!schema->getTypeDefinitions().empty());
        CHECK_EQUAL(uint64Type, schema->getTypeDefinitionNamed("UInt64"));

        auto serializedData = schema->serializeToBinaryData();
        auto deserializedSchema = Schema::deserializeFromBinaryData(serializedData);
        CHECK(deserializedSchema);
        CHECK(!deserializedSchema->getTypeDefinitions().empty());

        auto readUInt8Type = deserializedSchema->getTypeDefinitionNamed("UInt8");
        CHECK_EQUAL(SchemaTypeLayout::UnsignedInteger, readUInt8Type->layout);
        CHECK_EQUAL(1u, readUInt8Type->instanceSize);
        CHECK_EQUAL(1u, readUInt8Type->instanceAlignment);

        auto readUInt64Type = deserializedSchema->getTypeDefinitionNamed("UInt64");
        CHECK_EQUAL(SchemaTypeLayout::UnsignedInteger, readUInt64Type->layout);
        CHECK_EQUAL(8u, readUInt64Type->instanceSize);
        CHECK_EQUAL(8u, readUInt64Type->instanceAlignment);

        auto readStructureType = deserializedSchema->getTypeDefinitionNamed("TestStruct");
        CHECK_EQUAL(SchemaTypeLayout::Structure, readStructureType->layout);
        CHECK_EQUAL(16u, readStructureType->instanceSize);
        CHECK_EQUAL(8u, readStructureType->instanceAlignment);
        CHECK_EQUAL(readUInt8Type, readStructureType->getSlotNamed("byte")->type.lock());
        CHECK_EQUAL(0u, structureType->getSlotNamed("byte")->offset);
        CHECK_EQUAL(readUInt64Type, readStructureType->getSlotNamed("qword")->type.lock());
        CHECK_EQUAL(8u, structureType->getSlotNamed("qword")->offset);
    }

    TEST(PackedStructureType)
    {
        auto schema = std::make_shared<Schema> ();

        auto uint8Type = schema->createBasicType("UInt8", SchemaTypeLayout::UnsignedInteger, 1, 1);
        CHECK_EQUAL(SchemaTypeLayout::UnsignedInteger, uint8Type->layout);
        CHECK_EQUAL(1u, uint8Type->instanceSize);
        CHECK_EQUAL(1u, uint8Type->instanceAlignment);

        auto uint64Type = schema->createBasicType("UInt64", SchemaTypeLayout::UnsignedInteger, 8, 8);
        CHECK_EQUAL(SchemaTypeLayout::UnsignedInteger, uint64Type->layout);
        CHECK_EQUAL(8u, uint64Type->instanceSize);
        CHECK_EQUAL(8u, uint64Type->instanceAlignment);

        auto structureType = schema->createPackedStructureType("TestStruct");
        structureType->createSlot("byte", uint8Type);
        structureType->createSlot("qword", uint64Type);
        structureType->computeSlotLayout();
        CHECK_EQUAL(SchemaTypeLayout::PackedStructure, structureType->layout);
        CHECK_EQUAL(9u, structureType->instanceSize);
        CHECK_EQUAL(1u, structureType->instanceAlignment);
        CHECK_EQUAL(uint8Type, structureType->getSlotNamed("byte")->type.lock());
        CHECK_EQUAL(0u, structureType->getSlotNamed("byte")->offset);
        CHECK_EQUAL(uint64Type, structureType->getSlotNamed("qword")->type.lock());
        CHECK_EQUAL(1u, structureType->getSlotNamed("qword")->offset);

        CHECK(!schema->getTypeDefinitions().empty());
        CHECK_EQUAL(uint64Type, schema->getTypeDefinitionNamed("UInt64"));

        auto serializedData = schema->serializeToBinaryData();
        auto deserializedSchema = Schema::deserializeFromBinaryData(serializedData);
        CHECK(deserializedSchema);
        CHECK(!deserializedSchema->getTypeDefinitions().empty());

        auto readUInt8Type = deserializedSchema->getTypeDefinitionNamed("UInt8");
        CHECK_EQUAL(SchemaTypeLayout::UnsignedInteger, readUInt8Type->layout);
        CHECK_EQUAL(1u, readUInt8Type->instanceSize);
        CHECK_EQUAL(1u, readUInt8Type->instanceAlignment);

        auto readUInt64Type = deserializedSchema->getTypeDefinitionNamed("UInt64");
        CHECK_EQUAL(SchemaTypeLayout::UnsignedInteger, readUInt64Type->layout);
        CHECK_EQUAL(8u, readUInt64Type->instanceSize);
        CHECK_EQUAL(8u, readUInt64Type->instanceAlignment);

        auto readStructureType = deserializedSchema->getTypeDefinitionNamed("TestStruct");
        CHECK_EQUAL(SchemaTypeLayout::PackedStructure, readStructureType->layout);
        CHECK_EQUAL(9u, readStructureType->instanceSize);
        CHECK_EQUAL(1u, readStructureType->instanceAlignment);
        CHECK_EQUAL(readUInt8Type, readStructureType->getSlotNamed("byte")->type.lock());
        CHECK_EQUAL(0u, structureType->getSlotNamed("byte")->offset);
        CHECK_EQUAL(readUInt64Type, readStructureType->getSlotNamed("qword")->type.lock());
        CHECK_EQUAL(1u, structureType->getSlotNamed("qword")->offset);
    }

    TEST(UnionType)
    {
        auto schema = std::make_shared<Schema> ();

        auto uint8Type = schema->createBasicType("UInt8", SchemaTypeLayout::UnsignedInteger, 1, 1);
        CHECK_EQUAL(SchemaTypeLayout::UnsignedInteger, uint8Type->layout);
        CHECK_EQUAL(1u, uint8Type->instanceSize);
        CHECK_EQUAL(1u, uint8Type->instanceAlignment);

        auto uint64Type = schema->createBasicType("UInt64", SchemaTypeLayout::UnsignedInteger, 8, 8);
        CHECK_EQUAL(SchemaTypeLayout::UnsignedInteger, uint64Type->layout);
        CHECK_EQUAL(8u, uint64Type->instanceSize);
        CHECK_EQUAL(8u, uint64Type->instanceAlignment);

        auto unionType = schema->createUnionType("TestUnion");
        unionType->createSlot("byte", uint8Type);
        unionType->createSlot("qword", uint64Type);
        unionType->computeSlotLayout();
        CHECK_EQUAL(SchemaTypeLayout::Union, unionType->layout);
        CHECK_EQUAL(8u, unionType->instanceSize);
        CHECK_EQUAL(8u, unionType->instanceAlignment);
        CHECK_EQUAL(uint8Type, unionType->getSlotNamed("byte")->type.lock());
        CHECK_EQUAL(0u, unionType->getSlotNamed("byte")->offset);
        CHECK_EQUAL(uint64Type, unionType->getSlotNamed("qword")->type.lock());
        CHECK_EQUAL(0u, unionType->getSlotNamed("qword")->offset);

        CHECK(!schema->getTypeDefinitions().empty());
        CHECK_EQUAL(uint64Type, schema->getTypeDefinitionNamed("UInt64"));

        auto serializedData = schema->serializeToBinaryData();
        auto deserializedSchema = Schema::deserializeFromBinaryData(serializedData);
        CHECK(deserializedSchema);
        CHECK(!deserializedSchema->getTypeDefinitions().empty());

        auto readUInt8Type = deserializedSchema->getTypeDefinitionNamed("UInt8");
        CHECK_EQUAL(SchemaTypeLayout::UnsignedInteger, readUInt8Type->layout);
        CHECK_EQUAL(1u, readUInt8Type->instanceSize);
        CHECK_EQUAL(1u, readUInt8Type->instanceAlignment);

        auto readUInt64Type = deserializedSchema->getTypeDefinitionNamed("UInt64");
        CHECK_EQUAL(SchemaTypeLayout::UnsignedInteger, readUInt64Type->layout);
        CHECK_EQUAL(8u, readUInt64Type->instanceSize);
        CHECK_EQUAL(8u, readUInt64Type->instanceAlignment);

        auto readStructureType = deserializedSchema->getTypeDefinitionNamed("TestUnion");
        CHECK_EQUAL(SchemaTypeLayout::Union, readStructureType->layout);
        CHECK_EQUAL(8u, readStructureType->instanceSize);
        CHECK_EQUAL(8u, readStructureType->instanceAlignment);
        CHECK_EQUAL(readUInt8Type, readStructureType->getSlotNamed("byte")->type.lock());
        CHECK_EQUAL(0u, unionType->getSlotNamed("byte")->offset);
        CHECK_EQUAL(readUInt64Type, readStructureType->getSlotNamed("qword")->type.lock());
        CHECK_EQUAL(0u, unionType->getSlotNamed("qword")->offset);
    }

}
