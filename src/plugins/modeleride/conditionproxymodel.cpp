#include "conditionproxymodel.h"
#include <treexmlmodel/treexmlhashmodel.h>
#include <metadatamodel/dbxmlstruct.h>

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

ConditionProxyModel::ConditionProxyModel(QObject *parent) :
    ModifyProxyModel(parent)
{
}

int ConditionProxyModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    if(!sourceModel())
        return 0;

    return 4;
}

QVariant ConditionProxyModel::data(const QModelIndex &proxyIndex, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        TreeXmlHashModel *xmlModel = qobject_cast<TreeXmlHashModel *>(sourceModel());
        QString tagRole = data(proxyIndex,TreeXmlModel::TagRole).toString();

        if (tagRole == DBFILTERBLOCKXML::BLOCK) {
            switch (proxyIndex.column()) {
            case 0:
                return QString("{Блок}");
                break;
            case 3:
                if (proxyIndex.row() < rowCount(proxyIndex.parent())-1)
                    return ModifyProxyModel::data(
                                proxyIndex.sibling(
                                    proxyIndex.row(),
                                    xmlModel->columnDisplayedAttr(
                                        DBFILTERBLOCKXML::BLOCK,
                                        DBFILTERBLOCKXML::LINKOF)),
                                role);
            default:
                return QVariant();
            }
        }

        if (tagRole == DBCONDITIONXML::COND) {
            switch (proxyIndex.column()) {
            case 0: {
                QVariant value = ModifyProxyModel::data(
                            proxyIndex.sibling(
                                proxyIndex.row(),
                                xmlModel->columnDisplayedAttr(
                                    DBCONDITIONXML::COND,
                                    DBCONDITIONXML::FIRSTATTR)),
                            role);
                return value;
            }
            case 1:
                return ModifyProxyModel::data(
                            proxyIndex.sibling(
                                proxyIndex.row(),
                                xmlModel->columnDisplayedAttr(
                                    DBCONDITIONXML::COND,
                                    DBCONDITIONXML::OPERATOR)),
                            role);
            case 2: {
                QVariant value = ModifyProxyModel::data(
                            proxyIndex.sibling(
                                proxyIndex.row(),
                                xmlModel->columnDisplayedAttr(
                                    DBCONDITIONXML::COND,
                                    DBCONDITIONXML::SECONDATTR)),
                            role);
                return value;
            }
            case 3:
                if (proxyIndex.row() < rowCount(proxyIndex.parent())-1)
                    return ModifyProxyModel::data(
                                proxyIndex.sibling(
                                    proxyIndex.row(),
                                    xmlModel->columnDisplayedAttr(
                                        DBCONDITIONXML::COND,
                                        DBCONDITIONXML::LINKOF)),
                                role);
            default:
                return QVariant();
            }
        }
    }

    if (role == TreeXmlModel::TagRole)
        return ModifyProxyModel::data(proxyIndex.sibling(proxyIndex.row(),0),role);

    if (role == Qt::DecorationRole && proxyIndex.column() == 0) {
        if (data(proxyIndex,TreeXmlModel::TagRole) == DBFILTERBLOCKXML::BLOCK)
            return QIcon(":/block");
        if (data(proxyIndex,TreeXmlModel::TagRole) == DBCONDITIONXML::COND)
            return QIcon(":/expression");
    }

    return QVariant();
}

bool ConditionProxyModel::setData(const QModelIndex &proxyIndex,
                                  const QVariant &value, int role)
{
    if (role == TreeXmlModel::TagRole)
        return ModifyProxyModel::setData(proxyIndex.sibling(proxyIndex.row(),0),value,role);

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        TreeXmlHashModel *xmlModel = qobject_cast<TreeXmlHashModel *>(sourceModel());

        if (data(proxyIndex,TreeXmlModel::TagRole)
                == DBFILTERBLOCKXML::BLOCK) {

            int column;
            switch (proxyIndex.column()) {
            case 3:
                column = xmlModel->columnDisplayedAttr(
                            DBFILTERBLOCKXML::BLOCK,
                            DBFILTERBLOCKXML::LINKOF);
                if (proxyIndex.row() == rowCount(proxyIndex.parent())-1) {
                    if (proxyIndex.row()==0)
                        return ModifyProxyModel::setData(
                                    proxyIndex.sibling(proxyIndex.row(),column),
                                    "И",
                                    role);
                    else
                        return ModifyProxyModel::setData(
                                    proxyIndex.sibling(proxyIndex.row(), column),
                                    data(proxyIndex.sibling(proxyIndex.row()-1, 3),
                                         role),
                                    role);
                } else {
                    return ModifyProxyModel::setData(
                                proxyIndex.sibling(proxyIndex.row(),column),
                                value,
                                role);
                }
            default:
                return false;
            }
        }

        if (data(proxyIndex,TreeXmlModel::TagRole)
                == DBCONDITIONXML::COND) {

            int column;
            switch (proxyIndex.column()) {
            case 0:
                return ModifyProxyModel::setData(
                            proxyIndex.sibling(
                                proxyIndex.row(),
                                xmlModel->columnDisplayedAttr(
                                    DBCONDITIONXML::COND,
                                    DBCONDITIONXML::FIRSTATTR)),
                            value,
                            role);
                break;
            case 1:
                return ModifyProxyModel::setData(
                            proxyIndex.sibling(
                                proxyIndex.row(),
                                xmlModel->columnDisplayedAttr(
                                    DBCONDITIONXML::COND,
                                    DBCONDITIONXML::OPERATOR)),
                            value,
                            role);
                break;
            case 2:
                return ModifyProxyModel::setData(
                            proxyIndex.sibling(
                                proxyIndex.row(),
                                xmlModel->columnDisplayedAttr(
                                    DBCONDITIONXML::COND,
                                    DBCONDITIONXML::SECONDATTR)),
                            value,
                            role);
                break;
            case 3:
                column = xmlModel->columnDisplayedAttr(
                            DBCONDITIONXML::COND,
                            DBCONDITIONXML::LINKOF);

                if (proxyIndex.row() == rowCount(proxyIndex.parent())-1) {
                    if (proxyIndex.row()==0)
                        return ModifyProxyModel::setData(
                                    proxyIndex.sibling(proxyIndex.row(),column),
                                    "И",
                                    role);
                    else
                        return ModifyProxyModel::setData(
                                    proxyIndex.sibling(proxyIndex.row(),column),
                                    data(proxyIndex.sibling(proxyIndex.row()-1, 3), role),
                                    role);
                } else
                    return ModifyProxyModel::setData(
                                proxyIndex.sibling(proxyIndex.row(),column),
                                value,
                                role);
            default:
                return false;
            }

        }
    }

    return ModifyProxyModel::setData(proxyIndex,value,role);
}

bool ConditionProxyModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (data(parent,TreeXmlModel::TagRole) == DBCONDITIONXML::COND)
        return false;

    return ModifyProxyModel::insertRows(row, count, parent);
}

QModelIndex ConditionProxyModel::insertLastRows(int row, int count, const QModelIndex &parent)
{
    if (data(parent,TreeXmlModel::TagRole) == DBCONDITIONXML::COND)
        return QModelIndex().child(-1,-1);

    return ModifyProxyModel::insertLastRows(row, count, parent);
}

}}
