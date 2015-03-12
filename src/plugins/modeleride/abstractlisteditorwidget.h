#ifndef ABSTRACTLISTEDITORWIDGET_H
#define ABSTRACTLISTEDITORWIDGET_H

#include <QWidget>
#include <QAbstractProxyModel>
#include <QAbstractItemView>

#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/modifyproxymodel.h>

using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {


class AbstractListEditorWidget : public QWidget
{
    Q_OBJECT
public:
    //! Конструктор класса
    explicit AbstractListEditorWidget(QWidget *parent = 0);

    //! Деструктор класса
    ~AbstractListEditorWidget();

    //! Установка модели для редактирования
    void setModel(TreeXmlHashModel *model);

     //! Возращает модель
    TreeXmlHashModel *model();

    //! Возращает прокси модели
    ModifyProxyModel* proxyModel();

    //! Проверка на возможность удаления
    static bool isRemove(const QModelIndex &proxyIndex);

    //! Установка виджета отображения данных
    virtual void setItemView(QAbstractItemView *itemView);

    //! Возращает виджет отображения данных
    virtual QAbstractItemView *itemView();

signals:
    //! Сигнал об изменении корневого индекса в прокси
    void proxyIndexChanged(const QModelIndex &index);

public slots:
    //! Добавление значений списка
    bool add(const QString &tag);

    //! Удаление значений списка
    void remove();

    //! Применение изменений значений списка
    void submit();

    //! Перевод в режим редактирования
    void edit(bool flag = true);

    //! Отмена изменений значений списка
    void revert();

    //! Установка родителя значений списка
    void setRootIndex(const QModelIndex &index);

private:

    //! Прокси модель для значений списка
    ModifyProxyModel* m_proxyModel;

    //! Модель структуры классов
    TreeXmlHashModel* m_model;

    //! Виджет отображения данных
    QAbstractItemView *m_itemView;
};

}}

#endif // ABSTRACTLISTEDITORWIDGET_H