#ifndef NUMERATORWIDGET_H
#define NUMERATORWIDGET_H

#include "ui_numeratorwidget.h"

#include <QDataWidgetMapper>
#include <QStringListModel>

#include <treexmlmodel/treexmlhashmodel.h>

#include "abstracteditorwidget.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования нумератора
/*! Диалог предназначен для редактирования нумератора
*/

class NumeratorWidget :  public AbstractEditorWidget, private Ui::NumeratorWidget
{
    Q_OBJECT

public:
    explicit NumeratorWidget(QWidget *parent = 0);

    //! Деструктор редактора нумератора
    virtual ~NumeratorWidget();

    //! Установка модели структуры классов
    void setModel(RTPTechGroup::XmlModel::TreeXmlHashModel *model);

    //! Проверка заполнено ли имя нумератора
    bool isEmpty();

public slots:
    //! Добавление нумратора
    void add();

    //! Установка текущего нумератора
    void setCurrent(const QModelIndex &index);

    //! Перевод нумератора в режим редактирования
    void edit(bool flag = true);

    //! Применение изменений нумератора
    void submit();

    //! Сообщение о неверном имени нумератора
    void validateNumeratorName(QValidator::State state) const;

signals:
    //! Сигнал о редактировании нумератора
    void edited(bool flag);
};

}}

#endif // NUMERATORWIDGET_H
