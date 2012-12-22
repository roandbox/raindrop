#include <QtGui>
#include <QtXml>
#include <QMessageBox>
#include <QUuid>
#include "tagxmlitem.h"
#include "treexmlmodel.h"

TreeXMLModel::TreeXMLModel(QDomNode document, QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootItem = new TagXMLItem(document, NULL);
    m_column = 1;
    m_insTag = "element";
}

TreeXMLModel::~TreeXMLModel()
{   
    m_hashValue.clear();
    m_hashField.clear();
    delete m_rootItem;
}

int TreeXMLModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return  m_column;
}

void TreeXMLModel::addTagFilter(QString tag)
{
    m_filterTags << tag;
}

void TreeXMLModel::clearTagsFilter()
{
    m_filterTags.clear();
}

void TreeXMLModel::addAttributeTag(QString tag)
{
    m_attrTags << tag;
}

void TreeXMLModel::clearAttributeTags(){
    m_attrTags.clear();
}

bool TreeXMLModel::isInherited(const QModelIndex &index) const
{
    TagXMLItem *item = toItem(index);
    return item->isInherited();
}

void TreeXMLModel::addHashField(QString tag, QString value, UniqueField unique)
{
    m_hashField[tag].insert(value,unique);
}

void TreeXMLModel::makeHashingOne(TagXMLItem *item, bool remove)
{
    QString tag = item->nodeName();
    foreach (const QString& attr, m_hashField.value(tag).keys()){
        if (remove)
            m_hashValue[tag][attr].remove(item->value(attr),item);
        else
            m_hashValue[tag][attr].insert(item->value(attr),item);
    }
}

bool TreeXMLModel::makeHashingData(const QModelIndex &index, QString &dataValue)
{
    // Обновление хэша контроля уникальности
    if (!isInherited(index)) {
        QString tag  = index.data(Qt::UserRole).toString();
        QString attr = fieldDisplayedAttr(tag,index.column());
        if (m_hashField[tag].contains(attr)){
            QModelIndex existIndex = indexHashField(tag,attr,dataValue);
            if (existIndex.isValid())
                if (existIndex!=index){
                    if (m_hashField[tag].value(attr) == TreeXMLModel::UniqueRename){
                        int position = dataValue.lastIndexOf(QRegExp("_\\d*$"));
                        int number = 1;
                        if (position != -1)
                            number = dataValue.mid(position+1).toInt()+1;
                        dataValue = dataValue.left(position)+QString("_%1").arg(number);
                    } else if (m_hashField[tag].value(attr) == TreeXMLModel::Unique) {
                        return false;
                    } else if (m_hashField[tag].value(attr) == TreeXMLModel::Uuid) {
                        dataValue = QUuid::createUuid().toString();
                    }
                }
            TagXMLItem *item = toItem(index);

            m_hashValue[tag][attr].remove(
                        item->node().toElement().attribute(attr),item);
            m_hashValue[tag][attr].insert(dataValue,item);
        }
    }
    return true;
}

void TreeXMLModel::insertUuid(const QModelIndex &index)
{
    foreach (QString attr,m_hashField.value(m_insTag).keys())
        if (m_hashField[m_insTag].value(attr)==TreeXMLModel::Uuid)
            toItem(index)->setValue(attr,QUuid::createUuid().toString());
}

void TreeXMLModel::makeHashing(TagXMLItem *item, bool remove)
{    
    makeHashingOne(item, remove);
    for (int row=0; row < item->count(m_filterTags); row++) {
        TagXMLItem *childItem = item->child(row,m_filterTags);
        makeHashing(childItem, remove);
    }
}


QModelIndex TreeXMLModel::indexHashField(QString tag, QString attrName, QVariant value) const
{
    int column = indexDisplayedAttr(tag,attrName);
    QModelIndex index = fromItem(m_hashValue[tag][attrName].value(value.toString()));
    return index.sibling(index.row(),column);
}

void TreeXMLModel::refreshHashing(const QModelIndex &index, bool remove)
{
    makeHashing(toItem(index),remove);
}

void TreeXMLModel::refreshHashingOne(const QModelIndex &index, bool remove)
{
    QString tag = index.data(Qt::UserRole).toString();
    QString attr = fieldDisplayedAttr(tag,index.column());
    if (m_hashField[tag].contains(attr)){
        if (remove )
            m_hashValue[tag][attr].remove(index.data().toString(),toItem(index));
        else
            m_hashValue[tag][attr].insert(index.data().toString(),toItem(index));
    }
}

void TreeXMLModel::addRelation(const QString &tag, const QString &attr,
                               const QString &linkTag, const QString &linkAttr)
{
    m_linkField[tag][attr][linkTag] = linkAttr;
}

QModelIndex TreeXMLModel::indexLink(const QModelIndex &index) const
{
    TagXMLItem *item = toItem(index);
    QString tag = item->nodeName();
    QString attrName = fieldDisplayedAttr(tag,index.column());

    if (m_linkField[tag].contains(attrName)) {
        foreach (QString linkTag,m_linkField[tag][attrName].keys()){
            QString linkAttr = m_linkField[tag][attrName][linkTag];
            foreach (QString attr,m_hashField.value(linkTag).keys())
                if (m_hashField[linkTag].value(attr) == TreeXMLModel::Uuid){
                    QModelIndex linkIndex = indexHashField(linkTag, attr, item->value(attrName));
                    int column = indexDisplayedAttr(linkTag,linkAttr);
                    return linkIndex.sibling(linkIndex.row(),column);
                }
        }
    }
    return QModelIndex();
}

bool TreeXMLModel::isAttribute(const QModelIndex &index) const
{
    TagXMLItem *item = toItem(index);
    foreach (QString tagName,m_attrTags)
        if (tagName == item->nodeName())
            return true;
    return false;
}

bool TreeXMLModel::isInsert(const QModelIndex &index) const
{
    TagXMLItem *item = toItem(index);

    if (index.isValid()){
        foreach (QString tagName,m_insertTags[item->nodeName()])
            if (tagName == m_insTag)
                return true;
        return false;
    } else {
        if (rowCount(index) == 0)
            return !isAttribute(index);
        else
            QMessageBox::warning(NULL,
                                 tr("Предупреждение"),
                                 tr("У дерева может быть только один корневой узел"));
    }
    return false;
}

bool TreeXMLModel::hasChildren(const QModelIndex &parent) const
{
    Q_UNUSED (parent);
    return true;
}

bool TreeXMLModel::unpackData(const QModelIndex &parent, QDataStream &stream, int row, bool move)
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
        } else {
            QVariant value;
            stream >> value;
            int column = indexDisplayedAttr(tag,nameAttr);

            QModelIndex existIndex = indexHashField(tag,nameAttr,value);
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

void TreeXMLModel::packData(const QModelIndex &parent, QDataStream &stream) const
{
    bool isFirstNode=true;

    for (int row=0;row<rowCount(parent);row++)
    {
        QModelIndex childIndex = parent.child(row,0);
        // Разделитель
        if (isFirstNode){
            stream << QString("{");
            isFirstNode = false;
        }

        if (childIndex.isValid())
            if (!isInherited(childIndex)) {
                stream << QString("^");
                QString tag = data(childIndex,Qt::UserRole).toString();
                stream << tag;
                for (int i = 0; i < columnCount(childIndex); i++) {
                    QString attrName = fieldDisplayedAttr(tag,i);
                    if (!attrName.isEmpty()){
                        stream << attrName;
                        stream << childIndex.sibling(childIndex.row(),i).data();
                    }
                }
                if (!isAttribute(childIndex))
                    packData(childIndex,stream);
            }
    }

    if (!isFirstNode)
        stream << QString("}");
}

void TreeXMLModel::addDisplayedAttr(QString nameAttr, QStringList value, QIcon icon)
{
    if (value.count()>m_column)
        m_column = value.count();
    m_displayedAttr[nameAttr] = value;

    if (!icon.isNull())
        m_displayedIcon[nameAttr] = icon;
}

void TreeXMLModel::addInsertTags(QString tag,QStringList value)
{
    m_insertTags.insert(tag,value);
}

int TreeXMLModel::indexDisplayedAttr(QString nameAttr, QString fieldName) const
{
    for (int i=0;i<m_displayedAttr[nameAttr].count();i++){
        if (m_displayedAttr[nameAttr].at(i)==fieldName)
            return i;
    }
    return -1;
}

QString TreeXMLModel::fieldDisplayedAttr(QString nameAttr, int column) const
{
    if (m_displayedAttr[nameAttr].count()<=column)
        return QString("");
    return m_displayedAttr[nameAttr].at(column);
}

void TreeXMLModel::removeDisplayedAttr(QString nameAttr)
{
    bool flagcolumn = false;
    if (m_displayedAttr[nameAttr].count() == m_column)
        flagcolumn = true;

    m_displayedAttr.remove(nameAttr);
    m_displayedIcon.remove(nameAttr);

    if (flagcolumn){
        m_column = 1;
        foreach (QStringList diplayedAttr, m_displayedAttr.values())
            if (diplayedAttr.count()>m_column)
                m_column = diplayedAttr.count();
    }
}

QVariant TreeXMLModel::data(const QModelIndex &index, int role) const
{
    TagXMLItem *item = toItem(index);
    QString tag = item->nodeName();

    if (role == Qt::DecorationRole)
        if (!m_displayedIcon[tag].isNull()&&index.column()==0)
            return m_displayedIcon[tag];

    if (role == Qt::UserRole){
        return tag;
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole){
        if (m_displayedAttr[tag].isEmpty())
            return tag;

        if (index.column()<m_displayedAttr[tag].count()){
            QString attrName = fieldDisplayedAttr(tag,index.column());
            if (attrName == "parent"){
                QDomNode nodeParent = item->node().parentNode();
                QString parentTag = nodeParent.nodeName();
                if (!nodeParent.isElement())
                    return QVariant();

                // Отображает в качестве родителя первое поле
                foreach (QString attr,m_hashField.value(parentTag).keys())
                    if (m_hashField[parentTag].value(attr) == TreeXMLModel::Uuid){
                        if (role == Qt::DisplayRole)
                            attr = m_linkField[tag][attrName][parentTag];
                        return nodeParent.toElement().attribute(attr);
                    }
            }

            if (role == Qt::DisplayRole){
                QModelIndex link = indexLink(index);
                if (link.isValid())
                    return link.data();
            }
                /*if (m_linkField[tag].contains(attrName)) {
                    foreach (QString linkTag,m_linkField[tag][attrName].keys()){
                        QString linkAttr = m_linkField[tag][attrName][linkTag];
                        foreach (QString attr,m_hashField.value(linkTag).keys())
                            if (m_hashField[linkTag].value(attr) == TreeXMLModel::Uuid){
                                QModelIndex linkIndex = indexHashField(linkTag, attr, item->value(attrName));
                                int column = indexDisplayedAttr(linkTag,linkAttr);
                                return linkIndex.sibling(linkIndex.row(),column).data();
                            }
                    }
                }*/
            return item->value(attrName);
        }
    }
    return QVariant();
}

void TreeXMLModel::updateModifyRow(int emptyRowAttr, const QModelIndex &parent)
{
    int rowCount = this->rowCount(parent);
    for (int i=0;i<rowCount;i++){
        QModelIndex index = parent.child(i,0);
        if (!isAttribute(index)){
            updateModifyRow(emptyRowAttr,index);
            int row = this->rowCount(index)-emptyRowAttr-1;
            QModelIndex updateIndex = index.child(row,0);
            emit dataChanged(updateIndex,updateIndex.sibling(updateIndex.row(),columnCount(updateIndex)));
        }
    }
}

bool TreeXMLModel::setData(const QModelIndex &index, const QVariant &value,
                           int role)
{
    if (role != Qt::EditRole) return false;

    TagXMLItem *item = toItem(index);

    if (index.column()>=m_displayedAttr[item->nodeName()].count())
        return false;

    QString attrName = fieldDisplayedAttr(item->nodeName(),index.column());

    if (attrName.isEmpty())
        return false;

    QString dataValue = value.toString();
    if (!makeHashingData(index,dataValue))
        return false;

    item->setValue(attrName,dataValue);
    emit dataChanged(index,index);

    if (isAttribute(index)){
        int emptyRowAttr = 0;
        QModelIndex parent = index.parent();
        for (int i=index.row()+1;i<this->rowCount(parent);i++){
            QModelIndex idx = parent.child(i,parent.column());
            if (isAttribute(idx))
                emptyRowAttr++;
        }
        updateModifyRow(emptyRowAttr,parent);
    }

    return true;
}

TagXMLItem *TreeXMLModel::rootItem()
{
    return m_rootItem;
}

Qt::ItemFlags TreeXMLModel::flags(const QModelIndex &index) const
{
    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled |
                Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    else
        return Qt::ItemIsDropEnabled;
}

QVariant TreeXMLModel::headerData(int section, Qt::Orientation orientation,
                                  int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        if (m_header[section].isNull())
            return QString("%1").arg(section);
        else
            return  m_header[section];
    }
    return  QVariant();
}

bool TreeXMLModel::setHeaderData(int section, Qt::Orientation orientation,
                                 const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    m_header[section] = value.toString();
    emit headerDataChanged(orientation, section, section);

    return true;
}

QModelIndex TreeXMLModel::index(int row, int column, const QModelIndex &parent)
const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TagXMLItem *parentItem = toItem(parent);
    TagXMLItem *childItem = parentItem->child(row,m_filterTags,m_attrTags);

    if (childItem){
        return createIndex(row, column, childItem);
    } else
        return QModelIndex();
}

QModelIndex TreeXMLModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    TagXMLItem *childItem = toItem(child);
    TagXMLItem *parentItem = childItem->parent();

    return fromItem(parentItem);
}

int TreeXMLModel::rowCount(const QModelIndex &parent) const
{
    TagXMLItem *parentItem = toItem(parent);

    return parentItem->count(m_filterTags,m_attrTags);
}

bool TreeXMLModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row);

    TagXMLItem *parentItem = toItem(parent);
    if (!isInsert(parent))
        return false;
    bool success = true;

    int position = parentItem->count(m_filterTags);

    for (int i=0;i<count;i++)
        success = parentItem->insertChild(m_insTag) && success;

    updateInsertRows(position,count,parent);

    // Добавление корнего узла
    if (parent.isValid())
        m_lastInsRow = parent.child(position+count-1,0);
    else
        m_lastInsRow = index(position+count-1,0,parent);

    for (int i=0;i<count;i++) {
        insertUuid(index(position+i,0,parent));
    }

    return success;
}

void TreeXMLModel::updateInsertRows(int row,int count, const QModelIndex &parent)
{
    QModelIndex curIndex = parent.child(row,0);
    if (isAttribute(curIndex))
        for (int i=0;i<this->rowCount(parent);i++){
            QModelIndex index = parent.child(i,0);
            if (!isAttribute(index))
                updateInsertRows(this->rowCount(index)-count,count,index);
        }
    beginInsertRows(parent,row,row+count-1);
    endInsertRows();
}

void TreeXMLModel::updateRemoveRows(int emptyRowAttr,int count, const QModelIndex &parent)
{
    // Если атрибут то обновляем по дереву наследования
    int rowCount = this->rowCount(parent);
    for (int i=0;i<rowCount;i++){
        QModelIndex index = parent.child(i,0);
        if (!isAttribute(index)){
            updateRemoveRows(emptyRowAttr,count,index);
            int row = this->rowCount(index)-emptyRowAttr+count-1;
            beginRemoveRows(index,row,row+count-1);
            endRemoveRows();
        }
    }
}

bool TreeXMLModel::removeRows(int row, int count, const QModelIndex &parent)
{
    TagXMLItem *parentItem = toItem(parent);

    if  (!parentItem->checkRemoveChild(row))
        return false;

    // Удаление хэша уникальности
    for (int j=row; j < row+count; j++){
        QModelIndex index = parent.child(j,0);
        if (!isInherited(index))
            makeHashing(toItem(index),true);
    }

    beginRemoveRows(parent,row,row+count-1);
    for (int i=count-1;i>=0;i--)
        parentItem->removeChild(row+i);
    endRemoveRows();

    int emptyRowAttr = 0;
    for (int i=row+1-count;i<this->rowCount(parent);i++){
        QModelIndex index = parent.child(i,0);

        if (isAttribute(index))
            emptyRowAttr++;
    }
    updateRemoveRows(emptyRowAttr,count,parent);

    return true;
}

QModelIndex TreeXMLModel::lastInsertRow()
{
    return m_lastInsRow;
}

bool TreeXMLModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                                int row, int column, const QModelIndex &parent)
{
    if (!parent.isValid())
        return false;

    if (action == Qt::IgnoreAction)
        return true;
    if (!data->hasFormat("application/classxmlmodel"))
        return false;
    if (column >= columnCount(parent))
        return false;
    if (isAttribute(parent))
        return false;

    QByteArray encodedData = data->data("application/classxmlmodel");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);

    return unpackData(parent,stream,row);
    return unpackData(parent,stream,row,
                      data->parent() == this
                      && action == Qt::MoveAction);
}


void TreeXMLModel::setInsTagName(QString tagName)
{
    m_insTag = tagName;
}

TagXMLItem *TreeXMLModel::toItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TagXMLItem *item = static_cast<TagXMLItem*>(index.internalPointer());
        if (item) return item;
    }
    return m_rootItem;
}

QModelIndex TreeXMLModel::fromItem(TagXMLItem *item) const
{
    if (!item || item == m_rootItem)
        return QModelIndex();

    int row = item->parent()->childNumber(item,m_filterTags,m_attrTags);

    return createIndex(row , 0, item);
}

Qt::DropActions TreeXMLModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions TreeXMLModel::supportedDragActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList TreeXMLModel::mimeTypes() const
{
    QStringList types;
    types << "application/classxmlmodel";
    return types;
}

QMimeData *TreeXMLModel::mimeData(const QModelIndexList &indexes)
{
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (const QModelIndex& index,indexes){
        if (index.isValid())
            if (!isInherited(index)){
                stream << QString("^");
                QString tag = data(index,Qt::UserRole).toString();
                stream << tag;
                for (int i = 0; i < columnCount(index); i++) {
                    QString attrName = fieldDisplayedAttr(tag,i);
                    if (!attrName.isEmpty()){
                        stream << attrName;
                        stream << index.sibling(index.row(),i).data();
                    }
                }
                if (!isAttribute(index))
                    packData(index,stream);
            }
    }

    QMimeData *mimeData = new QMimeData();
    mimeData->setData("application/classxmlmodel", encodedData);
    mimeData->setParent(this);
    return mimeData;
}
