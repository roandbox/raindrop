#include <QtCore/QtGlobal>

#if defined(TREEDOCKWIDGET_LIBRARY)
#  define TREEDOCKWIDGETLIB Q_DECL_EXPORT
#else
#  define TREEDOCKWIDGETLIB Q_DECL_IMPORT
#endif
