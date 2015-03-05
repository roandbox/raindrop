#include "classwidget.h"
#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"
#include <QStringListModel>
#include <QToolTip>
#include "regexpvalidator.h"

using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

ClassWidget::ClassWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    RegExpValidator *validator =
            new RegExpValidator(QRegExp("^[A-Za-z]{1}[A-Za-z0-9_]{0,26}|^[A-Za-z]{0}"));
    lineEditClassName->setValidator(validator);
    connect(validator,SIGNAL(stateChanged(QValidator::State)),
            this,SLOT(validateClassName(QValidator::State)));

    m_mapper = new QDataWidgetMapper();
    m_mapper->setItemDelegate(new XmlDelegate(this));
    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    m_typeClassModel = new QStringListModel();
    const QStringList classType = (QStringList()
                                   << DBCLASSTYPEXML::STANDART
                                   << DBCLASSTYPEXML::SYSTEM
                                   << DBCLASSTYPEXML::USER
                                   );
    m_typeClassModel->setStringList(classType);

    lineEditClassParent->setReadOnly(true);

    connect(toolButtonAddClass,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonDelClass,SIGNAL(clicked()),this,SLOT(remove()));
    connect(pushButtonPropSave,SIGNAL(clicked()),this,SLOT(submit()));
    connect(pushButtonPropCancel,SIGNAL(clicked()),this,SLOT(revert()));
    connect(toolButtonEditClass,SIGNAL(clicked()),this,SLOT(edit()));
    m_oldIndex = QModelIndex();
}

ClassWidget::~ClassWidget()
{
    delete lineEditClassName->validator();
    delete m_typeClassModel;
    delete m_mapper;
}

void ClassWidget::setModel(TreeXmlHashModel *model)
{
    m_model = model;
    connect(m_model,SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this,SLOT(rowsRemoved(QModelIndex,int,int)));
    m_mapper->setModel(m_model);

    comboBoxClassType->setModel(m_typeClassModel);

    m_mapper->addMapping(lineEditClassName,
                         model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::NAME));
    m_mapper->addMapping(lineEditClassAlias,
                         model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::ALIAS));

    m_mapper->addMapping(comboBoxClassType,
                         model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::TYPE));

    m_mapper->addMapping(lineEditClassParent,
                         model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::PARENT));

    m_mapper->addMapping(checkBoxAbsClass,
                         model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::ISABSTARCT));
    m_mapper->addMapping(checkBoxActiveClass,
                         model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::ISACTIVE));

    m_mapper->addMapping(checkBoxContextClass,
                         model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::ISCONTEXT));

    m_mapper->addMapping(plainTextEditShowAttr,
                         model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::TEMPLATE));
    m_mapper->addMapping(pushButtonIcon,
                         model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                    DBCLASSXML::ICON));
}

void ClassWidget::add()
{
    m_oldIndex = m_model->index(m_mapper->currentIndex(),
                                0,m_mapper->rootIndex());
    QModelIndex srcIndex =  m_oldIndex.parent();
    QModelIndex srcCurrentIndex =
            m_model->insertLastRows(0,1,srcIndex,DBCLASSXML::CLASS);
    if (srcCurrentIndex.isValid()){
        setCurrent(srcCurrentIndex);
        comboBoxClassType->setCurrentIndex(0);
        edit(true);
    }
}

// Метод совпадает с bool ModelerIDEPlug::isRemoveClass(const QModelIndex &srcIndex)
bool ClassWidget::isRemove(QModelIndex srcIndex)
{
    const TreeXmlHashModel *model = dynamic_cast<const TreeXmlHashModel *>(srcIndex.model());
    if (!model)
        return false;

    bool success = true;
    QString msg;

    QString tag = srcIndex.data(TreeXmlModel::TagRole).toString();
    QStringList tags;
    tags << tag;
    if (model->rowCount(srcIndex,tags)) {
        msg += tr("Необходимо удалить классы-потомки.\n\n");
        if (success)
            success = false;
    }

    QString fieldId = model->uuidAttr(tag);
    if (fieldId.isEmpty())
        return true;

    QString guid =  srcIndex.sibling(srcIndex.row(),
                                     model->columnDisplayedAttr(
                                         tag,fieldId))
            .data().toString();

    foreach (TreeXmlHashModel::TagWithAttr tagWithAttr,
             model->fromRelation(tag))
    {
        int number = 0;

        QModelIndex linkIndex = model->indexHashAttr(
                    tagWithAttr.tag,
                    tagWithAttr.attr,
                    guid,
                    number
                    );

        while (linkIndex.isValid()) {
            QModelIndex linkParent = linkIndex.parent();
            if (linkParent.sibling(linkIndex.parent().row(),0)!= srcIndex){
                QString parentName;
                QString name;
                if (linkIndex.data(TreeXmlModel::TagRole) == DBCLASSXML::CLASS) {
                    name = tr("класс ")
                            + linkIndex.sibling(linkIndex.row(),
                                                model->columnDisplayedAttr(
                                                    DBCLASSXML::CLASS,
                                                    DBCLASSXML::NAME)
                                                ).data().toString();
                } else {
                    if (linkParent.data(TreeXmlModel::TagRole) == DBCOMPXML::COMP)
                        parentName = tr(" принадлежащий составу ")
                                + linkParent.sibling(
                                    linkParent.row(),
                                    model->columnDisplayedAttr(
                                        DBCOMPXML::COMP,
                                        DBCOMPXML::NAME)
                                    ).data().toString();
                    else
                        parentName = tr(" принадлежащий классу ")
                                + linkParent.sibling(
                                    linkParent.row(),
                                    model->columnDisplayedAttr(
                                        DBCLASSXML::CLASS,
                                        DBCLASSXML::NAME)
                                    ).data().toString();

                    if  (linkIndex.data(TreeXmlModel::TagRole) == DBCOMPXML::COMP)
                        name = tr("состав ")
                                + linkIndex.sibling(linkIndex.row(),
                                                    model->columnDisplayedAttr(
                                                        DBCOMPXML::COMP,
                                                        DBCOMPXML::NAME)
                                                    ).data().toString();
                    else
                        name = tr("атрибут ")
                                + linkIndex.sibling(linkIndex.row(),
                                                    model->columnDisplayedAttr(
                                                        DBATTRXML::ATTR,
                                                        DBATTRXML::NAME)
                                                    ).data().toString();

                }
                msg += QString(tr("Необходимо удалить %1%2.\n\n")).
                        arg(name).arg(parentName);
                if (success)
                    success = false;
            }
            number++;
            linkIndex = model->indexHashAttr(
                        tagWithAttr.tag,
                        tagWithAttr.attr,
                        guid,
                        number
                        );
        }
    }
    if (!success) {
        QMessageBox msgBox;
        msgBox.setText(tr("Удаление данного объекта не воможно."));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setDetailedText(msg);
        msgBox.setWindowTitle(tr("Предупреждение"));
        msgBox.exec();
    }
    return success;
}

void ClassWidget::remove()
{
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());

    if (!isRemove(srcIndex))
        return;

    m_mapper->revert();

    QModelIndex index = srcIndex.parent();
    m_mapper->setRootIndex(index.parent());
    m_mapper->setCurrentModelIndex(index);

    m_model->removeRow(srcIndex.row(),srcIndex.parent());
    emit dataRemoved(srcIndex);
}


void ClassWidget::setCurrent(const QModelIndex &index)
{
    m_mapper->setRootIndex(index.parent());
    emit currentIndexChanged(index);
    m_mapper->setCurrentModelIndex(index);

    int indexType = comboBoxClassType->findText(modelData(DBCLASSXML::CLASS,
                                                          DBCLASSXML::TYPE,
                                                          index).toString());
    comboBoxClassType->setCurrentIndex(indexType);
    edit(false);
}

bool ClassWidget::isEdit()
{
    return groupBoxClass->isEnabled();
}

void ClassWidget::edit(bool flag)
{
    if (isEdit()==flag)
        return;

    if (lineEditClassName->text().isEmpty()){
        toolButtonAddClass->setDisabled(true);
        flag = true;
    }else
        toolButtonAddClass->setEnabled(true);

    groupBoxClass->setEnabled(flag);
    pushButtonPropSave->setEnabled(flag);
    pushButtonPropCancel->setEnabled(flag);
    toolButtonEditClass->setDisabled(flag);
}

void ClassWidget::submit()
{
    QModelIndex existIndex = m_model->indexHashAttr(DBCLASSXML::CLASS,
                                                     DBCLASSXML::NAME,
                                                     lineEditClassName->text());
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());

    if (existIndex.isValid()){
        if (existIndex.sibling(existIndex.row(),0)!=srcIndex){
            QMessageBox::warning(this,tr("Предупреждение"),
                                 tr("Класс с таким именем уже существует"));
            return;
        }
    }

    m_mapper->submit();
    edit(false);
    if (!removeEmpty())
        emit dataChanged(srcIndex);
}

void ClassWidget::revert()
{
    m_mapper->revert();
    edit(false);
    removeEmpty();
}

void ClassWidget::rowsRemoved(const QModelIndex &index, int start, int end)
{
    Q_UNUSED (start)
    Q_UNUSED (end)
    if (index == m_mapper->rootIndex() && m_mapper->currentIndex()==-1)
        emit dataRemoved(QModelIndex());
}

void ClassWidget::validateClassName(QValidator::State state) const
{

    if(state != QValidator::Acceptable)
        QToolTip::showText(lineEditClassName->mapToGlobal(QPoint(0,5)),
                           tr("Имя класса должно содержать только латинские\n"
                              "символы и цифры длиной не более 27 символов"));
    else
        QToolTip::hideText();
}

QVariant ClassWidget::modelData(const QString &tag, const QString &attr, const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                             tag,attr)).data();
}

bool ClassWidget::removeEmpty()
{
    if (lineEditClassName->text().isEmpty()){
        if (m_oldIndex.isValid()){
            QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
            m_mapper->revert();
            setCurrent(m_oldIndex);
            m_model->removeRow(srcIndex.row(),srcIndex.parent());
            m_oldIndex = QModelIndex();
        }else {
            remove();
        }
        return true;
    }
    return false;
}

}}
