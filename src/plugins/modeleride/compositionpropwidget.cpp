#include "compositionpropwidget.h"
#include "xmldelegate.h"
#include "dbxmlstruct.h"

CompositionPropWidget::CompositionPropWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    m_mapper = new QDataWidgetMapper();
    m_mapper->setItemDelegate(new XmlDelegate(this));
    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    lineEditSrcClass->setReadOnly(true);

    connect(toolButtonAdd,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonDel,SIGNAL(clicked()),this,SLOT(remove()));
    connect(pushButtonPropSave,SIGNAL(clicked()),this,SLOT(submit()));
    connect(pushButtonPropCancel,SIGNAL(clicked()),this,SLOT(revert()));
    connect(toolButtonEdit,SIGNAL(clicked()),this,SLOT(edit()));
    m_oldIndex = -1;
}

CompositionPropWidget::~CompositionPropWidget()
{
    delete m_mapper;
}

void CompositionPropWidget::setModel(TreeXMLModel *model)
{
    m_model = model;
    connect(m_model,SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this,SLOT(rowsRemoved(QModelIndex,int,int)));

    m_mapper->setModel(m_model);

    comboBoxDestClass->setModel(m_model);

    m_mapper->addMapping(lineEditName,
                         model->indexDisplayedAttr(DBCOMPXML::COMP,
                                                   DBCOMPXML::NAME));
    m_mapper->addMapping(lineEditDesc,
                         model->indexDisplayedAttr(DBCOMPXML::COMP,
                                                   DBCOMPXML::DESCRIPTION));

    m_mapper->addMapping(lineEditDirectDesc,
                         model->indexDisplayedAttr(DBCOMPXML::COMP,
                                                   DBCOMPXML::DIRECTDESCRIPTION));

    m_mapper->addMapping(lineEditInverseDesc,
                         model->indexDisplayedAttr(DBCOMPXML::COMP,
                                                   DBCOMPXML::INVERSEDESCRIPTION));
    m_mapper->addMapping(lineEditSrcClass,
                         model->indexDisplayedAttr(DBCOMPXML::COMP,
                                                   DBCOMPXML::PARENT));

    m_mapper->addMapping(comboBoxDestClass,
                         model->indexDisplayedAttr(DBCOMPXML::COMP,
                                                   DBCOMPXML::CLASS));
}

void CompositionPropWidget::add()
{
    m_oldIndex = m_mapper->currentIndex();
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),
                                          0,m_mapper->rootIndex()).parent();

    m_model->setInsTagName(DBCOMPXML::COMP);
    if (m_model->insertRow(0,srcIndex)){
        QModelIndex srcCurrentIndex = m_model->lastInsertRow();
        setCurrent(srcCurrentIndex);
        edit(true);
    }
}

void CompositionPropWidget::remove()
{
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
    m_mapper->revert();
    setCurrent(srcIndex.parent());

    emit dataRemoved(srcIndex);
    m_model->removeRow(srcIndex.row(),srcIndex.parent());
}


void CompositionPropWidget::setCurrent(QModelIndex index)
{
    if (m_mapper->rootIndex() == index.parent() &&
            index.row() == m_mapper->currentIndex())
        return;

    m_mapper->setRootIndex(index.parent());
    m_mapper->setCurrentModelIndex(index);
    edit(false);

    emit currentIndexChanged(index);
}

void CompositionPropWidget::edit(bool flag)
{
    if (groupBoxProp->isEnabled()==flag)
        return;

    if (lineEditName->text().isEmpty()){
        toolButtonAdd->setDisabled(true);
        flag = true;
    }else
        toolButtonAdd->setEnabled(true);

    groupBoxProp->setEnabled(flag);
    pushButtonPropSave->setEnabled(flag);
    pushButtonPropCancel->setEnabled(flag);
    toolButtonEdit->setDisabled(flag);
}

void CompositionPropWidget::submit()
{
    m_mapper->submit();
    removeEmpty();
    edit(false);
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
    emit dataChanged(srcIndex);
}

void CompositionPropWidget::revert()
{
    m_mapper->revert();
    removeEmpty();
    edit(false);
}

void CompositionPropWidget::rowsRemoved(QModelIndex index, int start, int end)
{
    if (index == m_mapper->rootIndex()){
        if (m_oldIndex > end)
            m_oldIndex = m_oldIndex - end-start-1;
        else if (m_oldIndex >= start && m_oldIndex <= end){
            m_oldIndex = -1;
        }
    }

    if (index == m_mapper->rootIndex() && m_mapper->currentIndex()==-1 && m_oldIndex <0)
        emit dataRemoved(QModelIndex());
}


QVariant CompositionPropWidget::modelData(QString typeName, QString attr, const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->indexDisplayedAttr(
                             typeName,attr)).data();
}

void CompositionPropWidget::removeEmpty()
{
    if (lineEditName->text().isEmpty()){
        if (m_oldIndex>=0){
            m_model->removeRow(m_mapper->currentIndex(),
                               m_mapper->rootIndex());
            setCurrent(m_mapper->rootIndex().child(m_oldIndex,0));
            m_oldIndex = -1;
        }else {
            remove();
            return;
        }
    }
}
