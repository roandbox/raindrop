#ifndef MODELERIDE_H
#define MODELERIDE_H

#include <QObject>

#include <plugin/iplugin.h>
#include <treexmlmodel/treexmlhashmodel.h>
#include <metadatamodel/classmodelxml.h>
#include <guiextension/dockwidget.h>

#include "classtreeview.h"
#include "treefilterproxymodel.h"



namespace RTPTechGroup {
namespace ModelerIde {

#define RECENTCOUNT 9 //! Количество последних моделей

//! Плагин среды разработки структуры классов
/*! Плагин предназначен для разработки структуры классов
    с сохранением ее XML. Имеется возможность разработанную
    структуру классов опубликовать на сервере БД
*/
class ModelerIDEPlug:
        public QObject,
        public IPlugin
{
    Q_OBJECT

    Q_INTERFACES(IPlugin)
    Q_PLUGIN_METADATA(IID IPlugin_iid FILE "modelerideplug.json")

public:

// IPlugin
    //! Получение имени плагина
    QString name() {return tr(APP_NAME);}

    //! Получение иконки плагина
    QIcon icon() {return QIcon(":/modeleride");}

    //! Описание продукта
    QString product() {return tr(APP_PRODUCT);}

    //! Авторские права
    QString copyright() {return tr(APP_COPYRIGHT);}

    //! Описание плагина
    QString descript() {return tr(APP_DESCRIPTION);}

    //! Категория в которой состоит плагин
    QString category() {return tr("");}

    //! Версия плагина
    QString version() {return APP_VERSION;}

    //! Производитель плагина
    QString vendor() {return tr(APP_COMPANY);}

// ModelerIDEPlug
    //! Конструктор плагина
    explicit ModelerIDEPlug(QObject *parent = 0);

    //! Деструктор плагина
    virtual ~ModelerIDEPlug();

    //! Получение Модели данных плагина
    RTPTechGroup::XmlModel::TreeXmlHashModel *model();

    //! Создание класса
    void createClassModel(QDomDocument document = QDomDocument());

    //! Получение имени по индексу
    QString dataName(const QModelIndex &index);

    //! Получение индетификатора по индексу
    QString dataId(const QModelIndex &index);

public slots:
    //! Слот вызова окна редактирования
    void showPropEditor(const QModelIndex &indexSource, bool editable);

    //! Слот вызова окна редактирования
    void showPropEditor(const QModelIndex &indexSource);

    //! Слот вызова редактирования в окне редактирования
    void editPropEditor(const QModelIndex &indexSource);

    //! Закрытие окна по индексу
    void closePropEditor(const QModelIndex& index);

    //! Создание новой структуры классов
    void newClassModel();

    //! Открытие структуры классов
    void openClassModel();

    //! Сохранение структуры классов
    void saveClassModel();

    //! Сохранение структуры классов как ...
    void saveAsClassModel();

    //! Публикация структуры классов на сервере БД
    void publishClassModel(const QModelIndex &index);

    //! Публикация структуры классов на сервере БД
    void publishClassModel();

    //! Закрытие структуры классов
    void closeClassModel();

    //! Добавление элемента модели класса
    void add();

    //! Удаление элемента модели класса
    void remove();

    //! Устанавливает активность QAction "Сохранить модель"
    void actionSaveEnable();

    //! Открытие последней модели
    void openRecentModel();

    //! Очистка списка последних моделей
    void clearRecentModels();

    //! Обновление списка последних моделей
    void updateRecentModels();

    //! Чтение списка последних файлов
    void readRecentFiles();

    //! Запись списка последних файлов
    void writeRecentFiles();

private:
    //! Имя файла структуры
    QString m_fileName;

    //! Модель дерева классов
    RTPTechGroup::MetaDataModel::ClassModelXml *m_model;

    //! Дерево классов
    ClassTreeView *m_treeClassView;

    //! Список последних файлов моделей
    QStringList m_recentFiles;

    //! Команда создания структуры классов
    QAction *m_actionNewModel;

    //! Команда сохранения структуры классов
    QAction *m_actionSaveModel;

    //! Команда сохранения структуры классов как...
    QAction *m_actionSaveAsModel;

    //! Команда открытия структуры классов
    QAction *m_actionOpenModel;

    //! Команда открытия последних моделей
    QAction *m_actionRecentModel[RECENTCOUNT];

    //! Команда очистки списка последних моделей
    QAction *m_actionClearRecentModels;

    //! Команда публикации структуры классов в БД
    QAction *m_actionPublishModel;

    //! Команда закрытия структуры классов
    QAction *m_actionCloseModel;

    //! Стыковый компонент для модели метаданных
    RTPTechGroup::Widgets::DockWidget *m_dockWidget;
};

}}

#endif
