#ifndef CLASSMODELXML_H
#define CLASSMODELXML_H

#include "metadatamodelglobal.h"
#include <treexmlmodel/treexmlhashmodel.h>

namespace  RTPTechGroup {
namespace  MetaDataModel {

//! Класс модели дерева классов
/*! Данный класс предназначен для создания дерева метаданных в XML формате
    на основе TreeXmlHashModel. Позволяет отображать указанные наследуемые тэги.
*/

class METADATAMODELLIB ClassModelXml : public RTPTechGroup::XmlModel::TreeXmlHashModel
{
    Q_OBJECT
public:
    //! Конструктор класса
    explicit ClassModelXml(QDomNode document = QDomNode(), QObject *parent = 0);

    //! Деструктор класса
    virtual ~ClassModelXml();

    //! Вставка строки
    QModelIndex insertLastRows (int row, int count,
                                const QModelIndex & parent, QString tag = "element");

    //! Удаление строки
    bool removeRows (int row, int count, const QModelIndex & parent);

    //! Проверка на возможность удаления элементов
    bool isRemove(const QModelIndex &srcIndex);

private:
    //! Добавления дегов для работы
    void initTagFilters();

    //! Добавление видимых атрибутов
    void initDisplayedAttrs();

    //! Добавление тэгов доступных для вставки
    void initInsertTags();

    //! Добавление жэшей для атрибутов и контроля их уникальности
    void initHashAttrs();

    //! Добавление связей между атрибутами
    void initRelations();

    //! Инициализация модели классов
    void initModel();
};

}}

#endif // CLASSMODEL_H
