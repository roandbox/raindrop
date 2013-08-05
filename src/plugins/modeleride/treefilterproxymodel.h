#ifndef TREEFILTERPROXYMODEL_H
#define TREEFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QStringList>
#include <QSet>

//! Фильрующая древовидная прокси модель
/*! Прокси модель дерева. предназначена для сортировки и фильтрации
 */

class TreeFilterProxyModel : public QSortFilterProxyModel
{
public:
    //! Конструктор прокси модели
    TreeFilterProxyModel();

    //! Добавление тэгов для отображения
    void addVisibleTag(const QString &tag);

    //! Удаление тэгов для отображения
    void removeVisibleTag(const QString &tag);

    //! Список тэгов для отображения
    QSet<QString> visibleTags();

    //! Обработчик окрнчания перетаскивания данных путем Drag and Drop
    bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                      int row, int column, const QModelIndex &parent);

    //! Возращает объект с упакованными данными по списку индексов
    QMimeData *mimeData(const QModelIndexList &indexes) const;

    //! Возращает список типов, которые могут быть упакованы
    QStringList mimeTypes() const;

    //! Возращает поддерживаемые моделью операции Drop
    Qt::DropActions supportedDropActions() const;

    //! Возращает поддерживаемые моделью операции Drag
    Qt::DropActions supportedDragActions() const;

protected:
    //! Фильтр строчек
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

    //! Фильтр строчек по дереву
    bool filterAcceptsRowItself(int source_row, const QModelIndex &source_parent) const;

    //! Поиск детей удолетворящих фильтру
    bool hasAcceptedChildren(int source_row, const QModelIndex &source_parent) const;

    //! Сортировка. Порядок: Класс, Состав, Атрибут, Алфавит
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:

    //! Копирование элемента в исходной модели
    bool copySourceIndex(const QModelIndex &srcIndex, const QModelIndex &destIndex,
                         bool recursively);

    //! Перемещение элемента в исходной модели
    bool moveSourceIndex(const QModelIndex &srcIndex, const QModelIndex &destIndex,
                         bool recursively, bool first = false);

    //! Список тэгов для фильтрации
    QSet<QString> m_filterTags;
};

#endif // TREEFILTERPROXYMODEL_H
