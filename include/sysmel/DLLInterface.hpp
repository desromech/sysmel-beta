#ifndef SYSMEL_DLL_INTERFACE_HPP
#define SYSMEL_DLL_INTERFACE_HPP

#ifdef _WIN32
#   ifdef BUILD_SYSMEL_COMPILER_LIB
#       define SYSMEL_COMPILER_LIB_EXPORT __declspec(dllexport)
#   else
#       define SYSMEL_COMPILER_LIB_EXPORT __declspec(dllimport)
#   endif
#else
#       define SYSMEL_COMPILER_LIB_EXPORT
#endif

#endif //SYSMEL_DLL_INTERFACE_HPP