#include <QtXml>
#include <QMessageBox>
#include <QUuid>
#include "tagxmlitem.h"
#include "treexmlhashmodel.h"

TreeXmlHashModel::TreeXmlHashModel(QDomNode document, QObject *parent)
    : TreeXmlModel(document, parent)
{

}

TreeXmlHashModel::~TreeXmlHashModel()
{   
    m_hashValue.clear();
    m_hashAttr.clear();
}

void TreeXmlHashModel::addHashAttr(const QString &tag, const QString &value, UniqueAttr unique)
{
    m_hashAttr[tag].insert(value,unique);
}

void TreeXmlHashModel::makeHashingOne(TagXmlItem *item, bool remove)
{
    QString tag = item->nodeName();
    foreach (const QString& attr, m_hashAttr[tag].keys()){
        if (remove)
            m_hashValue[tag][attr].remove(item->value(attr),item);
        else
            m_hashValue[tag][attr].insert(item->value(attr),item);
    }
}

bool TreeXmlHashModel::makeHashingData(const QModelIndex &index, QString &dataValue)
{
    // Обновление хэша контроля уникальности
    if (!isInherited(index)) {
        QString tag  = index.data(Qt::UserRole).toString();
        QString attr = displayedAttr(tag,index.column());
        if (m_hashAttr[tag].contains(attr)){
            QModelIndex existIndex = indexHashAttr(tag,attr,dataValue);
            while (existIndex.isValid() && existIndex!=index) {
                if (m_hashAttr[tag][attr] == TreeXmlHashModel::UniqueRename){
                    int position = dataValue.lastIndexOf(QRegExp("_\\d*$"));
                    int number = 1;
                    if (position != -1)
                        number = dataValue.mid(position+1).toInt()+1;
                    dataValue = dataValue.left(position)+QString("_%1").arg(number);
                } else if (m_hashAttr[tag][attr] == TreeXmlHashModel::Unique) {
                    return false;
                } else if (m_hashAttr[tag][attr] == TreeXmlHashModel::Uuid) {
                    dataValue = QUuid::createUuid().toString();
                } else
                    break;
                existIndex = indexHashAttr(tag,attr,dataValue);
            }
            TagXmlItem *item = toItem(index);

            m_hashValue[tag][attr].remove(
                        item->node().toElement().attribute(attr),item);
            m_hashValue[tag][attr].insert(dataValue,item);
        }
    }
    return true;
}

void TreeXmlHashModel::insertUuid(const QModelIndex &index)
{
    QString attr = uuidAttr(insTagName());
    if (!attr.isEmpty()) {
        TagXmlItem *item = toItem(index);
        QString value = QUuid::createUuid().toString();
        item->setValue(attr,value);
        m_hashValue[insTagName()][attr].insert(value,item);
    }
}

void TreeXmlHashModel::makeHashing(TagXmlItem *item, bool remove)
{    
    makeHashingOne(item, remove);
    for (int row=0; row < item->count(tagsFilter()); row++) {
        TagXmlItem *childItem = item->child(row,tagsFilter());
        makeHashing(childItem, remove);
    }
}

QModelIndex TreeXmlHashModel::indexHashAttr(const QString &tag, const QString &attr,
                                         const QVariant &value, int number) const
{
    int column = columnDisplayedAttr(tag,attr);
    if (number <  m_hashValue[tag][attr].values(value.toString()).count()) {
        QModelIndex index = fromItem(
                    m_hashValue[tag][attr].values(
                        value.toString()).at(number)
                    );
        return index.sibling(index.row(),column);
    }
    return QModelIndex();
}

void TreeXmlHashModel::refreshHashing(const QModelIndex &index, bool remove)
{
    makeHashing(toItem(index),remove);
}

void TreeXmlHashModel::refreshHashingOne(const QModelIndex &index, bool remove)
{
    QString tag = index.data(Qt::UserRole).toString();
    QString attr = displayedAttr(tag,index.column());
    if (m_hashAttr[tag].contains(attr)){
        if (remove )
            m_hashValue[tag][attr].remove(index.data().toString(),toItem(index));
        else
            m_hashValue[tag][attr].insert(index.data().toString(),toItem(index));
    }
}

void TreeXmlHashModel::addRelation(const QString &tag, const QString &attr,
                               const QString &linkTag, const QString &linkAttr)
{
    m_linkAttr[tag][attr][linkTag] = linkAttr;
}

QList<TreeXmlHashModel::TagWithAttr> TreeXmlHashModel::fromRelation(const QString &linkTag,
                                                                      const QString &linkAttr)
{
    QList<TreeXmlHashModel::TagWithAttr> list;
    foreach (const QString &tag,m_linkAttr.keys())
        foreach (const QString &attr,m_linkAttr[tag].keys())
            foreach (const QString &lTag,m_linkAttr[tag][attr].keys())
                if (lTag == linkTag
                        && (m_linkAttr[tag][attr][lTag] == linkAttr || linkAttr.isEmpty()))
                {
                    TreeXmlHashModel::TagWithAttr tagWithAttr;
                    tagWithAttr.tag =  tag;
                    tagWithAttr.attr = attr;
                    list << tagWithAttr;
                }
    return list;
}

TreeXmlHashModel::TagWithAttr TreeXmlHashModel::toRelation(const QString &tag,
                                                             const QString &attr)
{
    TreeXmlHashModel::TagWithAttr tagWithAttr;
    foreach (const QString &linkTag,m_linkAttr[tag][attr].keys()) {
        tagWithAttr.tag =  linkTag;
        tagWithAttr.attr = m_linkAttr[tag][attr][linkTag];
        return tagWithAttr;
    }
    return tagWithAttr;
}

QModelIndex TreeXmlHashModel::indexLink(const QModelIndex &index) const
{
    TagXmlItem *item = toItem(index);
    QString tag = item->nodeName();
    QString attrName = displayedAttr(tag,index.column());

    if (m_linkAttr[tag].contains(attrName)) {
        foreach (const QString &linkTag,m_linkAttr[tag][attrName].keys()){
            QString linkAttr = m_linkAttr[tag][attrName][linkTag];
            QString attr = uuidAttr(linkTag);
            if (!attr.isEmpty()) {
                QModelIndex linkIndex = indexHashAttr(linkTag, attr, item->value(attrName));
                int column = columnDisplayedAttr(linkTag,linkAttr);
                return linkIndex.sibling(linkIndex.row(),column);
            }
        }
    }
    return QModelIndex();
}

bool TreeXmlHashModel::unpackData(const QModelIndex &parent, QDataStream &stream, int row, bool move)
{
    QString tag;
    QModelIndex index;
    bool nextTag = false;

    while (!stream.atEnd()) {
        QString nameAttr;
        stream >> nameAttr;
        if (nameAttr==QString("^")){
            stream >> tag;
            setInsTagName(tag);
            if (insertRow(row,parent))
                nextTag = false;
            else
                nextTag = true;
            index = lastInsertRow();
        } else if (nameAttr==QString("{")) {
            unpackData(lastInsertRow(),stream,row);
        } else if (nameAttr==QString("}")) {
            return true;
        } else if (!nextTag){
            QVariant value;
            stream >> value;
            int column = columnDisplayedAttr(tag,nameAttr);

            QModelIndex existIndex = indexHashAttr(tag,nameAttr,value);
            if (existIndex.isValid() && move)
                refreshHashingOne(existIndex,true);

            if (!setData(index.sibling(index.row(),column),value)){
                nextTag = true;
                removeRow(index.row(),index.parent());
            }
        }
    }
    return true;
}

QVariant TreeXmlHashModel::data(const QModelIndex &index, int role) const
{
    TagXmlItem *item = toItem(index);
    QString tag = item->nodeName();

    if ((role == Qt::DisplayRole || role == Qt::EditRole)
            && !displayedAttr(tag,0).isEmpty())
    {
        if (!displayedAttr(tag,index.column()).isEmpty()){
            QString attrName = displayedAttr(tag,index.column());
            if (attrName == "parent"){
                QDomNode nodeParent = item->node().parentNode();
                QString parentTag = nodeParent.nodeName();
                if (!nodeParent.isElement())
                    return QVariant();

                // Отображает в качестве родителя первое поле
                QString attr = uuidAttr(parentTag);
                if (!attr.isEmpty()) {
                    if (role == Qt::DisplayRole)
                        attr = m_linkAttr[tag][attrName][parentTag];
                    return nodeParent.toElement().attribute(attr);
                }
            }

            if (role == Qt::DisplayRole){
                QModelIndex link = indexLink(index);
                if (link.isValid())
                    return link.data();
            }

            return item->value(attrName);
        }
        return QVariant();
    }

    return TreeXmlModel::data(index, role);
}

bool TreeXmlHashModel::setData(const QModelIndex &index, const QVariant &value,
                           int role)
{
    if (!TreeXmlModel::setData(index,value,role))
        return false;

    TagXmlItem *item = toItem(index);
    QString attrName = displayedAttr(item->nodeName(),index.column());
    QString dataValue = value.toString();
    if (attrName == QString("parent"))
        dataValue = data(index,Qt::EditRole).toString();

    if (!makeHashingData(index,dataValue))
        return false;

    return true;
}

bool TreeXmlHashModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row);

    TagXmlItem *parentItem = toItem(parent);
    if (!isInsert(parent))
        return false;

    int position = parentItem->count(tagsFilter());

    if (!TreeXmlModel::insertRows(row, count, parent))
        return false;

    for (int i=0;i<count;i++) {
        insertUuid(index(position+i,0,parent));
    }

    return true;
}

bool TreeXmlHashModel::removeRows(int row, int count, const QModelIndex &parent)
{
    TagXmlItem *parentItem = toItem(parent);

    if  (!parentItem->checkRemoveChild(row))
        return false;

    // Удаление хэша уникальности
    for (int i=row; i < row+count; i++){
        QModelIndex childIndex = index(i,0,parent);
        if (!isInherited(childIndex))
            makeHashing(toItem(childIndex),true);
    }

    return TreeXmlModel::removeRows(row, count, parent);
}

QString TreeXmlHashModel::uuidAttr(const QString &tag) const
{
    foreach (QString attr,m_hashAttr[tag].keys())
        if (m_hashAttr[tag][attr]==TreeXmlHashModel::Uuid)
            return attr;
    return QString();
}
