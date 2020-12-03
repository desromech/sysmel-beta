#ifndef SYSMEL_COMPILER_PARSER_SOURCE_COLLECTION_HPP
#define SYSMEL_COMPILER_PARSER_SOURCE_COLLECTION_HPP
#pragma once

#include <string>
#include <memory>

namespace SysmelMoebius
{
namespace Compiler
{

/**
 * I represent the string data from a source collection.
 */
struct SourceCollection
{
    SourceCollection() {}
    SourceCollection(const std::string &ctext, const std::string &cname = "")
        : text(ctext), name(cname) {}

    std::string text;
    std::string name;
};

typedef std::shared_ptr<SourceCollection> SourceCollectionPtr;

} // End of namespace Compiler
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_PARSER_SOURCE_COLLECTION_HPP
