#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(UNILITY_LIB)
#  define UNILITY_EXPORT Q_DECL_EXPORT
# else
#  define UNILITY_EXPORT Q_DECL_IMPORT
# endif
#else
# define UNILITY_EXPORT
#endif
