#ifndef IDATABASEITEM_H
#define IDATABASEITEM_H

#include <QString>
#include <QUuid>

//! Элемент базы данных
class IDatabaseItem
{
    //! Возращает идентификатор элемента базы данных
    virtual QUuid id() = 0;

    //! Устанавливает идентификатор элемента базы данных
    virtual void setId(QUuid id) = 0;

    //! Возращает имя элемента базы данных
    virtual QString name() = 0;

    //! Устанавливает имя элемента базы данных
    virtual void setName(const QString &name) = 0;

    //! Возращает псевдоним элемента базы данных
    virtual QString alias() = 0;

    //! Устанавливает псевдоним элемента базы данных
    virtual void setAlias(const QString &alias) = 0;

    //! Проверка существования элемента базы данных
    virtual bool isExist() = 0;

    //! Создание элемента базы данных
    virtual bool create() = 0;

    //! Обновление элемента базы данных
    virtual bool update() = 0;

    //! Удаление элемента базы данных
    virtual bool remove() = 0;
};

#endif // IDATABASEITEM_H
