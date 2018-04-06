#ifndef __QTMREF_GLOBAL_H_
#define __QTMREF_GLOBAL_H_

#include <QtCore/qglobal.h>

#if defined(MLEQTMREF_LIBRARY)
#  define MLEQTMREFSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MLEQTMREFSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif /* MLEQTMREF_GLOBAL_H_ */
