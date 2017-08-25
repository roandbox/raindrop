#ifndef IDATABASEFILTER_H
#define IDATABASEFILTER_H

#include <idatabaseitem.h>
#include <idatabaseclass.h>
#include <idatabaseexpression.h>

class IDatabaseClass;

//! Фильтр класса базы данных
class IDatabaseFilter: public IDatabaseItem
{
public:
    //! Конструктор класса
    explicit IDatabaseFilter() {}

    //! Деструктор класса
    virtual ~IDatabaseFilter() {}

    //!  Возращает основной класс
    virtual IDatabaseClass *primaryClass() const {
        return m_primaryClass;
    }

    //! Устанавливает основной класс
    virtual void setPrimaryClass(IDatabaseClass *cls) {
        m_primaryClass = cls;
    }

    //!  Возращает второй класс
    virtual IDatabaseClass *secondClass() const {
        return m_secondClass;
    }

     //! Устанавливает второй класс
    virtual void setSecondClass(IDatabaseClass *cls) {
        m_secondClass = cls;
    }

    //! Возращает описание прямой связи
    virtual QString directDescrition() const {
        return m_directDescrition;
    }

    //! Устанавливает описание прямой связи
    virtual void setDirectDescrition(const QString &directDescrition) {
       m_directDescrition = directDescrition;
    }

    //! Возращает описание обратной связи
    virtual QString inverseDescrition() const {
        return m_inverseDescrition;
    }

    //! Устанавливает описание обратной связи
    virtual void setInverseDescrition(const QString &inverseDescrition) {
        m_inverseDescrition = inverseDescrition;
    }

    //! Установка выражения для фильтра
    virtual void setExpression(IDatabaseExpression *expr) {
        m_expression = expr;
    }

    //! Получение выражения фильтра
    virtual IDatabaseExpression *expression() const {
        return m_expression;
    }

private:
    //! Основной класс
    IDatabaseClass *m_primaryClass;

    //! Второй класс
    IDatabaseClass *m_secondClass;

    //! Описание прямой связи
    QString m_directDescrition;

    //! Описание обратной связи
    QString m_inverseDescrition;

    //! Выражение фильтра
    IDatabaseExpression *m_expression;
};

#endif
