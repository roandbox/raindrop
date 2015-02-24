#ifndef REFITEMWIDGET_H
#define REFITEMWIDGET_H

#include "ui_refitemwidget.h"

#include <QStringListModel>
#include <QDataWidgetMapper>

#include <treexmlmodel/modifyproxymodel.h>
#include <treexmlmodel/treexmlhashmodel.h>

#include "refproxymodel.h"

//! Диалог редактирования элементов справочника
/*! Диалог предназначен для редактирования элементов справочника
*/

class RefItemWidget : public QWidget, private Ui::RefItemWidget
{
    Q_OBJECT

public:
    //! Конструктор класса
    explicit RefItemWidget(QWidget *parent = 0);

    //! Деструктор класса
    virtual ~RefItemWidget();

    //! Установка модели для редактирования элементов справочника
    void setModel(TreeXmlHashModel *model);

    //! Возращает прокси модель  элементов справочника
    QAbstractProxyModel* proxyModel();

    //! Проверка на возможность удаления  элементов справочника
    static bool isRemove(const QModelIndex &srcIndex);

signals:
    //! Сигнал об изменении корневого индекса в прокси  элементов справочника
    void proxyIndexChanged(const QModelIndex &index);

public slots:
    //! Добавление элементов справочника
    void add();

    //! Добавление дочерних элементов справочника
    void addChild();

    //! Удаление  элементов справочника
    void remove();

    //! Применение изменений  элементов справочника
    void submit();

    //! Перевод в режим редактирования
    void edit(bool flag = true);

    //! Отмена изменений  элементов справочника
    void revert();

    //! Установка родителя  элементов справочника
    void setRootIndex(const QModelIndex &index);

private:

    //! Прокси модель для  элементов справочника
    RefProxyModel *m_refModel;

    //! Модель структуры классов
    TreeXmlHashModel* m_model;
};


#endif // REFITEMWIDGET_H
