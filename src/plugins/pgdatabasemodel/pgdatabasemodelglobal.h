#ifndef PGDATABASEMODELGLOBAL_H
#define PGDATABASEMODELGLOBAL_H
#if defined(PGDATABASEMODEL_LIBRARY)
#  define PGDATABASEMODELLIB Q_DECL_EXPORT
#else
#  define PGDATABASEMODELLIB Q_DECL_IMPORT
#endif
#endif // PGDATABASEMODELGLOBAL_H
