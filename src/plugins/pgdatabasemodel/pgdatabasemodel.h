#ifndef PGDATABASEMODEL_H
#define PGDATABASEMODEL_H

#include <QtSql/QSqlDatabase>
#include <databasemodel/idatabasemodel.h>
#include "pgdatabasemodelglobal.h"

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация модели базы данных PostgreSql
class PGDATABASEMODELLIB PgDatabaseModel : public IDatabaseModel
{

public:
    //! Конструктор класса
    explicit PgDatabaseModel(IDatabasePool *pool);

    //! Инициализирует модель базы данных
    IDatabaseError init(IDatabaseSession *session = nullptr);

// Получение доступа к элементам модели по имени
     //! Получение класса по имени
    IDatabaseClass *oneClass(const QString &name);

// Получение доступа к спискам элементов модели
    //! Список классов модели
    IDatabaseClasses *classList(const QString &baseClass = QString());

private:
    int m_number;
};

}}

#endif // PGDATABASEMODEL_H
