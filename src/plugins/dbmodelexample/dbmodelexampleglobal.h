#ifndef DBMODELEXAMPLEGLOBAL_H
#define DBMODELEXAMPLEGLOBAL_H
#if defined(DBMODELEXAMPLE_LIBRARY)
#  define DBMODELEXAMPLELIB Q_DECL_EXPORT
#else
#  define DBMODELEXAMPLELIB Q_DECL_IMPORT
#endif
#endif // DBMODELEXAMPLEGLOBAL_H