#ifndef IDATABASEPOOL_H
#define IDATABASEPOOL_H

#include <sqlextension/threadquery.h>

#include <QUuid>

//! Модель пул базы данных
class IDatabasePool
{

public:

    //! Деструктор класса
    virtual ~IDatabasePool() {}

    //! Резервирует sql запрос потока
    virtual RTPTechGroup::SqlExtension::ThreadQuery *acquire(QUuid threadUuid) = 0;

    //! Получение sql запроса потока
    virtual RTPTechGroup::SqlExtension::ThreadQuery *find(QUuid threadUuid) = 0;

    //! Возвращает sql запрос потока
    virtual void release(QUuid threadUuid) = 0;
};



#endif // IDATABASEPOOL_H