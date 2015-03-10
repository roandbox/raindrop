#include "abstractpropeditor.h"

#include <QMdiSubWindow>

namespace RTPTechGroup {
namespace ModelerIde {

AbstractPropEditor::AbstractPropEditor(QWidget *parent)
    : QWidget(parent)
{

}

AbstractPropEditor::~AbstractPropEditor()
{

}

void AbstractPropEditor::setModel(TreeXmlHashModel *model)
{
    m_model = model;
}

TreeXmlHashModel *AbstractPropEditor::model()
{
    return m_model;
}

void AbstractPropEditor::closeTab(const QModelIndex &index)
{
    Q_UNUSED(index);

    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());
    if (subWindow)
        subWindow->close();
}

QVariant AbstractPropEditor::modelData(const QString &tag, const QString &attr, const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                             tag,attr)).data();
}

}}
