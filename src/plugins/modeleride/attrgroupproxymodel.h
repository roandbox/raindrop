#ifndef ATTRGROUPPROXYMODEL_H
#define ATTRGROUPPROXYMODEL_H

#include <QAbstractListModel>

class AttrGroupProxyModel : public QAbstractListModel
{
    Q_OBJECT

public:
    //! Конструктор класса
    explicit AttrGroupProxyModel(QObject *parent = 0);

    //! Деструктор класаа
    virtual ~AttrGroupProxyModel();

    //! Возращает количество строк в индексе родителя
    int rowCount(const QModelIndex& parent) const;

    //! Возращает хранимые данные
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    //! Установка модели источника
    void setModel(QAbstractItemModel *srcModel);

    //! Получение модели источника
    QAbstractItemModel *sourceModel() const;

    //! Установка корневого индекса
    void setRootModelIndex(const QModelIndex &index);

    //! Получение корневого индекса
    QModelIndex rootModelIndex();

    //! Указание столбца с уникальными значениями
    void setUniqueColumn(int column);

    //! Возращает столбец с уникальными значениями
    int uniqueColumn() const;

public slots:
    //! Инициализация модели
    void reset();

private:
    int m_uniqueColumn;
    QList<QString> m_list;
    QAbstractItemModel *m_model;
    QPersistentModelIndex m_rootIndex;
};

#endif // ATTRGROUPPROXYMODEL_H