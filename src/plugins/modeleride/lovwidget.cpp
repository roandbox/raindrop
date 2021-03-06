#include "lovwidget.h"

#include <QMessageBox>
#include <QToolTip>

#include <metadatamodel/dbxmlstruct.h>

#include "regexpvalidator.h"

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

LovWidget::LovWidget(QWidget *parent) :
    AbstractEditorWidget(parent)
{
    setupUi(this);

    RegExpValidator *validator =
            new RegExpValidator(QRegExp("^[A-Za-z]{1}[A-Za-z0-9_]{0,26}|^[A-Za-z]{0}"));
    lineEditLovName->setValidator(validator);
    connect(validator, &RegExpValidator::stateChanged,
            this, &LovWidget::validateLovName);

    m_typeAttrModel = new QStringListModel();
    const QStringList attrTypeList = (QStringList()
                                       << DBATTRTYPEXML::BOOLEAN
                                       << DBATTRTYPEXML::BINARY
                                       << DBATTRTYPEXML::CHAR
                                       << DBATTRTYPEXML::DATE
                                       << DBATTRTYPEXML::DECIMAL
                                       << DBATTRTYPEXML::DOUBLE
                                       << DBATTRTYPEXML::INTEGER
                                       << DBATTRTYPEXML::STRING
                                       << DBATTRTYPEXML::TIME
                                       << DBATTRTYPEXML::TIMESHTAMP
                                       );
    m_typeAttrModel->setStringList(attrTypeList);

    connect(comboBoxLovType,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &LovWidget::changeType);
}

LovWidget::~LovWidget()
{
    delete lineEditLovName->validator();
    delete m_typeAttrModel;
}

bool LovWidget::isEmpty()
{
    return lineEditLovName->text().isEmpty();
}

bool LovWidget::add()
{
    return AbstractEditorWidget::add(DBLOVXML::LOV);
}

void LovWidget::setModel(TreeXmlHashModel *model)
{
    AbstractEditorWidget::setModel(model);

    dataMapper()->addMapping(lineEditLovName,
                             model->columnDisplayedAttr(DBLOVXML::LOV,
                                                        DBLOVXML::NAME));
    dataMapper()->addMapping(lineEditLovAlias,
                             model->columnDisplayedAttr(DBLOVXML::LOV,
                                                        DBLOVXML::ALIAS));
    dataMapper()->addMapping(comboBoxLovType,
                             model->columnDisplayedAttr(DBLOVXML::LOV,
                                                        DBLOVXML::TYPE));
    dataMapper()->addMapping(spinBoxLength,
                             model->columnDisplayedAttr(DBLOVXML::LOV,
                                                        DBLOVXML::MAXSTRLEN));
    dataMapper()->addMapping(spinBoxAccuracy,
                             model->columnDisplayedAttr(DBLOVXML::LOV,
                                                        DBLOVXML::ACCURACY));
    comboBoxLovType->setModel(m_typeAttrModel);
}

void LovWidget::setCurrent(const QModelIndex &index)
{
    AbstractEditorWidget::setCurrent(index);

    qint32 indexType = comboBoxLovType->findText(modelData(
                                                  DBLOVXML::LOV,
                                                  DBLOVXML::TYPE,
                                                  index).toString());
    comboBoxLovType->setCurrentIndex(indexType);
}

void LovWidget::edit(bool flag)
{
    if (isEmpty()) flag = true;
    groupBoxLov->setEnabled(flag);

    emit edited(flag);
}

void LovWidget::submit()
{
    QModelIndex existIndex = model()->indexHashAttr(DBLOVXML::LOV,
                                                    DBLOVXML::NAME,
                                                    lineEditLovName->text());
    QModelIndex srcIndex = model()->index(dataMapper()->currentIndex(),0,
                                          dataMapper()->rootIndex());

    if (existIndex.isValid()){
        if (existIndex.sibling(existIndex.row(),0)!=srcIndex){
            QMessageBox::warning(this,tr("Предупреждение"),
                                 tr("Список значений с таким именем уже существует"));
            return;
        }
    }

    AbstractEditorWidget::submit();
}

void LovWidget::validateLovName(QValidator::State state) const
{
    if(state != QValidator::Acceptable)
        QToolTip::showText(lineEditLovName->mapToGlobal(QPoint(0,5)),
                           tr("Имя списка значений должно содержать только латинские\n"
                              "символы и цифры длиной не более 27 символов"));
    else
        QToolTip::hideText();
}

void LovWidget::changeType(const int &index)
{
    auto typeName = comboBoxLovType->itemText(index);
    if (DBATTRTYPEXML::STRING == typeName) {
        spinBoxLength->setEnabled(true);
        spinBoxAccuracy->setEnabled(false);
    } else {
        spinBoxLength->setEnabled(false);
        spinBoxLength->setValue(0);
        spinBoxAccuracy->setEnabled(DBATTRTYPEXML::DECIMAL == typeName);
    }
}

}}
