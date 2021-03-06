#include "lovvaluewidget.h"
#include <metadatamodel/dbxmlstruct.h>
#include "lovdelegate.h"
#include <QMessageBox>

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

LovValueWidget::LovValueWidget(QWidget *parent) :
    AbstractModifyWidget(parent)
{
    setupUi(this);

    connect(toolButtonAddLovValue, &QToolButton::clicked, this, &LovValueWidget::add);
    connect(toolButtonDeleteLovValue, &QToolButton::clicked, this, &LovValueWidget::remove);

    AbstractModifyWidget::setItemView(tableViewLovValue);
    itemView()->setItemDelegate(new LovDelegate());
}

LovValueWidget::~LovValueWidget()
{
    delete itemView()->itemDelegate();
}

void LovValueWidget::setModel(TreeXmlHashModel *model)
{
    AbstractModifyWidget::setModel(model);

    proxyModel()->setHeaderData(0,  Qt::Horizontal, tr("Наименование"));
    proxyModel()->setHeaderData(1,  Qt::Horizontal, tr("Значение"));
    proxyModel()->setHeaderData(2,  Qt::Horizontal, tr("Список значений"));
    proxyModel()->setHeaderData(3,  Qt::Horizontal, tr("Идентификатор"));

    for (qint32 column = 2; column < 18; ++column)
        tableViewLovValue->setColumnHidden(column,true);

    tableViewLovValue->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableViewLovValue->horizontalHeader()->setDefaultSectionSize(200);
}

void LovValueWidget::add()
{
    AbstractModifyWidget::add(DBLOVVALUEXML::LOVVALUE);
}

void LovValueWidget::edit(bool flag)
{
    AbstractModifyWidget::edit(flag);
    toolButtonAddLovValue->setEnabled(flag);
    toolButtonDeleteLovValue->setEnabled(flag);
}

}}
