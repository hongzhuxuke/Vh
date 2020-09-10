#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(VHMAINTHREAD_LIB)
#  define VHMAINTHREAD_EXPORT Q_DECL_EXPORT
# else
#  define VHMAINTHREAD_EXPORT Q_DECL_IMPORT
# endif
#else
# define VHMAINTHREAD_EXPORT
#endif


extern "C" __declspec(dllexport) bool RegisterGlobalManager(void*);