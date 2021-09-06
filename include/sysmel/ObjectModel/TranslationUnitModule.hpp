#ifndef SYSMEL_COMPILER_OBJECT_MODEL_TRANSLATION_UNIT_MODULE_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_TRANSLATION_UNIT_MODULE_HPP
#pragma once

#include "Module.hpp"
#include "ModuleReference.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am a particular module in the system.
 */
class TranslationUnitModule : public SubtypeOf<Module, TranslationUnitModule>
{
public:
    static constexpr char const __typeName__[] = "TranslationUnitModule";

    virtual bool isTranslationUnitModule() const;

private:
    std::vector<ModuleReferencePtr> importedModules;
    std::vector<ModuleReferencePtr> indirectlyImportedModules;
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_TRANSLATION_UNIT_MODULE_HPP