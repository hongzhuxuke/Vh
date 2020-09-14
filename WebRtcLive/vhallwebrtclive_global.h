#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(VHALLWEBRTCLIVE_LIB)
#  define VHALLWEBRTCLIVE_EXPORT Q_DECL_EXPORT
# else
#  define VHALLWEBRTCLIVE_EXPORT Q_DECL_IMPORT
# endif
#else
# define VHALLWEBRTCLIVE_EXPORT
#endif


extern "C" __declspec(dllexport) bool RegisterGlobalManager(void*);