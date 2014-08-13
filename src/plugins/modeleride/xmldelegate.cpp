#include "xmldelegate.h"
#include "pushbuttonimage.h"
#include <QComboBox>
#include <QAbstractItemView>
#include <QLineEdit>
#include <treecombobox/treecombobox.h>
#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/modifyproxymodel.h>

XmlDelegate::XmlDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget*  XmlDelegate::createEditor (QWidget * parent, const QStyleOptionViewItem & option,
        const QModelIndex & index ) const
{
    return QStyledItemDelegate::createEditor(parent,option,index);
}

void XmlDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{

    TreeComboBox* treeComboBox = dynamic_cast<TreeComboBox*>(editor);
    if (treeComboBox) {
        treeComboBox->setDisplayText(
                    index.model()->data(index,Qt::DisplayRole).toString());

        treeComboBox->repaint();
        return;
    }

    QComboBox* comboBox = dynamic_cast<QComboBox*>(editor);
    if (comboBox) {

        ModifyProxyModel* modifyModel = dynamic_cast<ModifyProxyModel*>(comboBox->model());
        QAbstractProxyModel *proxyModel = dynamic_cast<QAbstractProxyModel*>(comboBox->model());

        TreeXmlHashModel* hashModel;
        if (modifyModel)
            hashModel = dynamic_cast<TreeXmlHashModel*>(modifyModel->sourceModel());
        else if (proxyModel)
            hashModel = dynamic_cast<TreeXmlHashModel*>(proxyModel->sourceModel());
        else
            hashModel = dynamic_cast<TreeXmlHashModel*>(comboBox->model());

        if (hashModel && hashModel == index.model()) {

            QModelIndex curIndex;
            if (modifyModel)
                curIndex = modifyModel->mapFromSource(hashModel->indexLink(index));
            else if (proxyModel)
                curIndex = proxyModel->mapFromSource(hashModel->indexLink(index));
            else
                curIndex = hashModel->indexLink(index);
            comboBox->setCurrentIndex(curIndex.row());
            return;
        }

        comboBox->setEditText(index.model()->data(index,Qt::EditRole).toString());
        return;
    }

    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(editor);
    if (lineEdit) {
        lineEdit->setText(index.model()->data(index,Qt::DisplayRole).toString());
        return;
    }

    PushButtonImage *pushButton = dynamic_cast<PushButtonImage*>(editor);
    if (pushButton) {
        QByteArray bytes = QByteArray::fromBase64(index.data().toByteArray());
        pushButton->setData(bytes);
        return;
    }

    QStyledItemDelegate::setEditorData(editor, index);
    return;

}

void XmlDelegate::setModelData( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index )const
{
    TreeComboBox* treeComboBox = dynamic_cast<TreeComboBox*>(editor);
    if (treeComboBox) {
        if (treeComboBox->currentModelIndex().isValid())
            model->setData(index,treeComboBox->currentModelIndex().data(),Qt::EditRole);
        return;
    }
    // Если QComboBox присваиваем текущий текст, а не индекс
    QComboBox* comboBox = dynamic_cast<QComboBox*>(editor);
    if (comboBox) {
        ModifyProxyModel* modifyModel = dynamic_cast<ModifyProxyModel*>(comboBox->model());
        QAbstractProxyModel *proxyModel = dynamic_cast<QAbstractProxyModel*>(comboBox->model());

        TreeXmlHashModel* hashModel;
        if (modifyModel)
            hashModel = dynamic_cast<TreeXmlHashModel*>(modifyModel->sourceModel());
        else if (proxyModel)
            hashModel = dynamic_cast<TreeXmlHashModel*>(proxyModel->sourceModel());
        else
            hashModel = dynamic_cast<TreeXmlHashModel*>(comboBox->model());


        if (hashModel && hashModel == model) {
            QString tag  = index.data(Qt::UserRole).toString();
            QString attr = hashModel->displayedAttr(tag, index.column());
            int column = hashModel->columnDisplayedAttr(hashModel->toRelation(tag,attr).tag,
                                                        hashModel->uuidAttr(tag));

            QModelIndex curIndex;
            if (modifyModel)
                curIndex = modifyModel->mapFromSource(comboBox->view()->currentIndex());
            else if (proxyModel)
                curIndex = proxyModel->mapFromSource(comboBox->view()->currentIndex());
            else
                comboBox->view()->currentIndex();

            curIndex = curIndex.sibling(curIndex.row(), column);
            if (curIndex.isValid())
                model->setData(index, curIndex.data(Qt::EditRole), Qt::EditRole);
            return;
        }
        model->setData(index,comboBox->currentText(),Qt::EditRole);
        return;
    }

    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(editor);
    if (lineEdit)
        if (lineEdit->isReadOnly()) {
            model->setData(index,index.data(Qt::EditRole),Qt::EditRole);
            return;
        }

    PushButtonImage *pushButton = dynamic_cast<PushButtonImage*>(editor);
    if (pushButton) {
        model->setData(index,pushButton->data().toBase64(),Qt::EditRole);
        return;
    }

    QStyledItemDelegate::setModelData(editor,model,index);
    return;
}

void XmlDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

void XmlDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);
}
