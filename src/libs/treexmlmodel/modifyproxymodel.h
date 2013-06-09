#ifndef MODIFYPROXYMODEL_H
#define MODIFYPROXYMODEL_H

#include <QAbstractProxyModel>
#include <QSet>

//! Класс прокси модель  для кэшеирования измений в модели источника
/*! Данный класс предназначен для кэшеирования изменений в модели-источнике
    с последующим их примением при вызове функции submitAll(). \n
    Пример использования:
    \code
    ModifyProxyModel *modifyModel = new ModifyProxyModel();
    modifyModel->setSourceModel(model);
    ...
    ... Изменение данных в прокси модели ...
    ...
    modifyModel->submitAll();
    \endcode
*/

class ModifyProxyModel : public QAbstractItemModel
{
    Q_OBJECT

public:

    //! Конструктор модели
    ModifyProxyModel(QObject* parent = 0);


    //! Возращает количество столбцов в индексе родителя
    int columnCount (const QModelIndex& parent = QModelIndex()) const;

    //! Возращает количество строк в индексе родителя
    int rowCount(const QModelIndex& parent) const;

    //! Возращает True если имеются потомки
    bool hasChildren(const QModelIndex &parent) const;


    //! Возращает индекс модели для строки и колонки
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const ;

    //! Возращает индекс родителя
    QModelIndex parent (const QModelIndex& index) const ;


    //! Преобразует индекс модели источника в текущую модель
    QModelIndex mapFromSource(const QModelIndex& index) const ;

    //! Преобразует индекс текущей модели в индекс модели источника
    QModelIndex mapToSource(const QModelIndex& index) const ;


    //! Возращает хранимые данные
    QVariant data(const QModelIndex &proxyIndex, int role = Qt::DisplayRole) const;

    //! Устанавливает значение для указанной записи
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    //! Установка модели источника
    void setSourceModel(QAbstractItemModel *srcModel);

    //! Получение модели источника
    QAbstractItemModel *sourceModel() const;


    //! Возвращает флаг записи
    Qt::ItemFlags flags(const QModelIndex &index) const;

    //! Возращает название заголовка
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    //! Устанавливает название заголовка
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole);


    //! Удаление строки
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    //! Вставка строки
    bool insertRows(int  row, int count, const QModelIndex &parent = QModelIndex());
    bool insertRows(int *row, int count, const QModelIndex &parent = QModelIndex());
    bool insertRow (int *row, const QModelIndex &parent);

    //! Проверка является ли строка вставленной
    bool isInsertRow(const QModelIndex &index) const;

    //! Установка скрытия удаленных строк
    void setHiddenRow(bool enabled);

    //! Проверка установлен ли режим скрытия удаленных строк
    bool isHiddenRow();

public slots:

    //! Применение изменений сделанных в прокси модели к модели-источнику
    bool submitAll();

    //! Слот обработки сигнала изменения данных источника
    void sourceDataChanged(const QModelIndex &left, const QModelIndex &right);

    //! Слот обработки сигнала удаления строк в источнике
    void sourceRowsRemoved(const QModelIndex &parent, int start, int end);

    //! Слот обработки сигнала перед удалением строк в источника
    void sourceRowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);

    //! Слот обработки сигнала вставки строк в источнике
    void sourceRowsInserted(const QModelIndex &parent, int start, int end);

private:

    //! Вставка строк в исходную модель
    bool insertSourceRows(const QPersistentModelIndex &parent,
                          const QPersistentModelIndex &sourceParent = QModelIndex());

    //! Модель-источник
    QAbstractItemModel *m_sourceModel;

    //! Список измененных значений
    QHash <QPersistentModelIndex, QHash<int, QVariant> > m_updatedRow;

    //! Список вставленных строк
    QHash <QPersistentModelIndex, QList<QPersistentModelIndex *> > m_insertedRow;

    //! Список удаленных строк
    QSet  <QPersistentModelIndex> m_removedRow;

    //! Список названий заголовков
    QMap<int, QString> m_header;

    //! Флаг скрытия удаленных строк
    bool m_hiddenRow;
} ;

#endif
