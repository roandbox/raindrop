#include <QMainWindow>
#include <QFileDialog>
#include <QMenu>

#include <imainwindow.h>

#include <mdiextarea/mdiextarea.h>
#include <treexmlmodel/tagxmlitem.h>
#include <treexmlmodel/tablexmlproxymodel.h>
#include <plugin/pluginmanager.h>
#include <metadatamodel/dbxmlstruct.h>

#include "modelerideplug.h"
#include "propclass.h"
#include "propfilter.h"
#include "propquantity.h"
#include "propquantitygroup.h"
#include "lovwidget.h"
#include "refgroupwidget.h"
#include "quantitygroupwidget.h"
#include "proplov.h"
#include "proprefgroup.h"
#include "propref.h"
#include "abstractpropeditor.h"
#include "proprole.h"
#include "propnumerator.h"

using namespace RTPTechGroup::Plugin;
using namespace RTPTechGroup::Widgets;
using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

ModelerIDEPlug::ModelerIDEPlug(QObject *parent):
    QObject(parent), IPlugin("IMainWindow IUndoGroup IClipboardStack")
{
    m_model = NULL;

    PluginManager *pluginManager = PluginManager::instance();

    m_treeClassView = new ClassTreeView();
    connect(m_treeClassView, &ClassTreeView::doubleClicked,
            this, static_cast<void (ModelerIDEPlug::*)
            (const QModelIndex &)>(&ModelerIDEPlug::showPropEditor));
    connect(m_treeClassView, &ClassTreeView::actionInserted,
            this, &ModelerIDEPlug::add);
    connect(m_treeClassView, &ClassTreeView::actionRemoved,
            this, &ModelerIDEPlug::remove);

    // Создание пунктов строки меню и кнопок панели исрументов
    IMainWindow *iMainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    m_actionNewModel = new QAction(QIcon(":newmodel"), tr("Новая модель"), this);
    connect(m_actionNewModel, &QAction::triggered, this, &ModelerIDEPlug::newClassModel);
    m_actionNewModel->setObjectName("actionNewModel");
    iMainWindow->addAction(tr("Редактор модели"),m_actionNewModel);

    m_actionOpenModel = new QAction(QIcon(":openmodel"), tr("Открыть модель..."), this);
    connect(m_actionOpenModel, &QAction::triggered, this, &ModelerIDEPlug::openClassModel);
    m_actionOpenModel->setObjectName("actionOpenModel");
    iMainWindow->addAction(tr("Редактор модели"),m_actionOpenModel);

    m_actionSaveModel = new QAction(QIcon(":savemodel"), tr("Сохранить модель"), this);
    connect(m_actionSaveModel, &QAction::triggered, this, &ModelerIDEPlug::saveClassModel);
    m_actionSaveModel->setDisabled(true);
    m_actionSaveModel->setObjectName("actionSaveModel");
    iMainWindow->addAction(tr("Редактор модели"),m_actionSaveModel);

    m_actionSaveAsModel = new QAction(QIcon(":savemodel"), tr("Сохранить модель как..."), this);
    connect(m_actionSaveAsModel, &QAction::triggered, this, &ModelerIDEPlug::saveAsClassModel);
    m_actionSaveAsModel->setDisabled(true);
    m_actionSaveAsModel->setObjectName("actionSaveAsModel");
    iMainWindow->addAction(tr("Редактор модели"),m_actionSaveAsModel);

    m_actionPublishModel = new QAction(QIcon(":publish"), tr("Опубликовать модель..."), this);
//    connect(m_actionPublishModel, &QAction::triggered, this, &ModelerIDEPlug::publishClassModel);
    m_actionPublishModel->setDisabled(true);
    m_actionPublishModel->setObjectName("actionPublishModel");
    iMainWindow->addAction(tr("Редактор модели"),m_actionPublishModel);

    m_actionCloseModel = new QAction(QIcon(":closemodel"), tr("Закрыть модель"), this);
    connect(m_actionCloseModel, &QAction::triggered, this, &ModelerIDEPlug::closeClassModel);
    m_actionCloseModel->setDisabled(true);
    m_actionCloseModel->setObjectName("actionCloseModel");
    iMainWindow->addAction(tr("Редактор модели"),m_actionCloseModel);

    m_dockWidget = new DockWidget();
    m_dockWidget->setObjectName("MetamodelDockWidget");
    m_dockWidget->setWidget(m_treeClassView);
    m_dockWidget->setWindowTitle(tr("Модель метаданных"));
    iMainWindow->addDockWidget(Qt::LeftDockWidgetArea, m_dockWidget);
}

ModelerIDEPlug::~ModelerIDEPlug()
{
    delete m_treeClassView;
    delete m_dockWidget;
    closeClassModel();
    delete m_actionSaveModel;
    delete m_actionSaveAsModel;
    delete m_actionNewModel;
    delete m_actionOpenModel;
    delete m_actionPublishModel;
    delete m_actionCloseModel;
}

void ModelerIDEPlug::actionSaveEnable()
{
    m_actionSaveModel->setEnabled(true);
}

TreeXmlHashModel *ModelerIDEPlug::model()
{
    return m_model;
}

void ModelerIDEPlug::createClassModel(QDomDocument document)
{
    m_model = new ClassModelXml(document, this);

    m_model->setHeaderData(0,  Qt::Horizontal, tr("Имя атрибута"));
    m_model->setHeaderData(1,  Qt::Horizontal, tr("Псевдоним"));
    m_model->setHeaderData(2,  Qt::Horizontal, tr("Тип"));
    m_model->setHeaderData(3,  Qt::Horizontal, tr("Длина строки"));
    m_model->setHeaderData(4,  Qt::Horizontal, tr("Ссылочный класс"));
    m_model->setHeaderData(5,  Qt::Horizontal, tr("Класс"));
    m_model->setHeaderData(6,  Qt::Horizontal, tr("ЕИ"));
    m_model->setHeaderData(7,  Qt::Horizontal, tr("По умолчанию"));
    m_model->setHeaderData(8,  Qt::Horizontal, tr("Нижняя граница"));
    m_model->setHeaderData(9,  Qt::Horizontal, tr("Верхняя гранница"));
    m_model->setHeaderData(10, Qt::Horizontal, tr("Список значений"));
    m_model->setHeaderData(11, Qt::Horizontal, tr("Группа"));
    m_model->setHeaderData(12, Qt::Horizontal, tr("Нулевые значения"));
    m_model->setHeaderData(13, Qt::Horizontal, tr("Уникальный"));
    m_model->setHeaderData(14, Qt::Horizontal, tr("Кандидат в ключ"));
    m_model->setHeaderData(15, Qt::Horizontal, tr("Идентификатор"));

    connect(m_model, &QAbstractItemModel::dataChanged,
            this, &ModelerIDEPlug::actionSaveEnable);
    connect(m_model, &QAbstractItemModel::rowsRemoved,
            this, &ModelerIDEPlug::actionSaveEnable);

    m_treeClassView->setModel(m_model);

    m_actionPublishModel->setEnabled(true);
    m_actionSaveModel->setEnabled(true);
    m_actionSaveAsModel->setEnabled(true);
    m_actionCloseModel->setEnabled(true);
}

void ModelerIDEPlug::add()
{
    QModelIndex indexSource = m_treeClassView->currentIndex();
    if (!indexSource.isValid())
        return;

    QModelIndex lastInsertRow;
    QString tagRole = indexSource.data(TreeXmlModel::TagRole).toString();
    if (tagRole == DBCLASSLISTXML::CLASSLIST || tagRole == DBCLASSXML::CLASS)
    {
        lastInsertRow =
                m_model->insertLastRows(0,1,indexSource,DBCLASSXML::CLASS);
        if (lastInsertRow.isValid()){
            qint32 column = m_model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                      DBCLASSXML::MODE);
            m_model->setData(lastInsertRow.sibling(lastInsertRow.row(),column),
                             DBACCESSMODEXML::USER);
            column = m_model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                      DBCLASSXML::TYPE);
            m_model->setData(lastInsertRow.sibling(lastInsertRow.row(),column),
                             DBCLASSTYPEXML::NORMAL);
        }
    } else if (tagRole == DBQUANTITYLISTXML::QUANTITYLIST) {
        lastInsertRow =
                m_model->insertLastRows(0,1,indexSource,DBQUANTITYGROUPXML::QUANTITYGROUP);
    } else if (tagRole == DBQUANTITYGROUPXML::QUANTITYGROUP) {
        lastInsertRow =
                m_model->insertLastRows(0,1,indexSource,DBQUANTITYXML::QUANTITY);
    } else if (tagRole == DBLOVLISTXML::LOVLIST) {
        lastInsertRow =
                m_model->insertLastRows(0,1,indexSource,DBLOVXML::LOV);
    } else if (tagRole == DBNUMERATORLISTXML::NUMERATORLIST) {
        lastInsertRow =
                m_model->insertLastRows(0,1,indexSource,DBNUMERATORXML::NUMERATOR);
    } else if (tagRole == DBREFLISTXML::REFLIST) {
        lastInsertRow =
                m_model->insertLastRows(0,1,indexSource,DBREFGROUPXML::REFGROUP);
    } else if (tagRole == DBREFGROUPXML::REFGROUP) {
        lastInsertRow =
                m_model->insertLastRows(0,1,indexSource,DBREFXML::REF);
    } else if (tagRole == DBROLELISTXML::ROLELIST) {
        lastInsertRow =
                m_model->insertLastRows(0,1,indexSource,DBROLEXML::ROLE);
    } else return;

    if (lastInsertRow.isValid()){
        m_treeClassView->setCurrentIndex(lastInsertRow);
        editPropEditor(lastInsertRow);
    }

}

QString ModelerIDEPlug::dataName(const QModelIndex& index)
{
    if (index.data(TreeXmlModel::TagRole) == DBCLASSXML::CLASS)
    {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBCLASSXML::CLASS,
                                 DBCLASSXML::NAME
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBQUANTITYXML::QUANTITY)
    {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBQUANTITYXML::QUANTITY,
                                 DBQUANTITYXML::NAME
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBCOMPXML::COMP)
    {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBCOMPXML::COMP,
                                 DBCOMPXML::NAME
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBFILTERXML::FILTER)
    {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBFILTERXML::FILTER,
                                 DBFILTERXML::NAME
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBQUANTITYGROUPXML::QUANTITYGROUP) {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBQUANTITYGROUPXML::QUANTITYGROUP,
                                 DBQUANTITYGROUPXML::NAME
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBLOVXML::LOV) {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBLOVXML::LOV,
                                 DBLOVXML::NAME
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBNUMERATORXML::NUMERATOR) {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBNUMERATORXML::NUMERATOR,
                                 DBNUMERATORXML::NAME
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBREFGROUPXML::REFGROUP) {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBREFGROUPXML::REFGROUP,
                                 DBREFGROUPXML::NAME
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBREFXML::REF)
    {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBREFXML::REF,
                                 DBREFXML::NAME
                                 )).data().toString();
    }

    return "";
}

QString ModelerIDEPlug::dataId(const QModelIndex &index)
{
    if (index.data(TreeXmlModel::TagRole) == DBCLASSXML::CLASS)
    {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBCLASSXML::CLASS,
                                 DBCLASSXML::ID
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBQUANTITYXML::QUANTITY)
    {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBQUANTITYXML::QUANTITY,
                                 DBQUANTITYXML::ID
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBCOMPXML::COMP)
    {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBCOMPXML::COMP,
                                 DBCOMPXML::ID
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBFILTERXML::FILTER)
    {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBFILTERXML::FILTER,
                                 DBFILTERXML::ID
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBQUANTITYGROUPXML::QUANTITYGROUP) {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBQUANTITYGROUPXML::QUANTITYGROUP,
                                 DBQUANTITYGROUPXML::ID
                                 )).data().toString();
    }


    if (index.data(TreeXmlModel::TagRole) == DBLOVXML::LOV) {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBLOVXML::LOV,
                                 DBLOVXML::ID
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBNUMERATORXML::NUMERATOR) {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBNUMERATORXML::NUMERATOR,
                                 DBNUMERATORXML::ID
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBREFGROUPXML::REFGROUP) {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBREFGROUPXML::REFGROUP,
                                 DBREFGROUPXML::ID
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBREFXML::REF)
    {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBREFXML::REF,
                                 DBREFXML::ID
                                 )).data().toString();
    }
    return "";
}

void ModelerIDEPlug::remove()
{
    QModelIndex currentIndex = m_treeClassView->currentIndex();

    if (currentIndex.isValid()){
        if (!m_model->isRemove(currentIndex))
            return;
        m_model->removeRow(currentIndex.row(),currentIndex.parent());
    } else
        QMessageBox::warning(NULL,tr("Предупреждение"),
                             tr("Невозможно удалить узел, поскольку он не выбран."));
}

void ModelerIDEPlug::showPropEditor(const QModelIndex &indexSource, bool editable)
{
    if (!indexSource.isValid())
        return;

    PluginManager *pluginManager = PluginManager::instance();
    IMainWindow *mainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    QString tagRole = indexSource.data(TreeXmlModel::TagRole).toString();
    QString subWindowName;

    if (tagRole == DBCLASSXML::CLASS)
        subWindowName = "PropClass::" + this->dataId(indexSource);
    else if (tagRole == DBFILTERXML::FILTER)
        subWindowName = "PropFilter::" + this->dataId(indexSource);
    else if (tagRole == DBQUANTITYXML::QUANTITY)
        subWindowName = "PropQuantity::" + this->dataId(indexSource);
    else if (tagRole == DBQUANTITYGROUPXML::QUANTITYGROUP)
        subWindowName = "PropQuantityGroup::" + this->dataId(indexSource);
    else if (tagRole == DBLOVXML::LOV)
        subWindowName = "PropLov::" + this->dataId(indexSource);
    else if (tagRole == DBNUMERATORXML::NUMERATOR)
        subWindowName = "PropNumerator::" + this->dataId(indexSource);
    else if (tagRole == DBREFGROUPXML::REFGROUP)
        subWindowName = "PropRefGroup::" + this->dataId(indexSource);
    else if (tagRole == DBREFXML::REF)
        subWindowName = "PropRef::" + this->dataId(indexSource);
    else if (tagRole == DBROLEXML::ROLE)
        subWindowName = "PropRole::" + this->dataId(indexSource);
    else
        return;

    QMdiSubWindow *subWindow = mainWindow->setActiveSubWindow(subWindowName);
    AbstractPropEditor *propEditor;
    if (!subWindow) {
        if (tagRole == DBCLASSXML::CLASS) {
            PropClass *propClass = new PropClass();
            connect(propClass, &PropClass::editFilter,
                    this, &ModelerIDEPlug::editPropEditor);
            propEditor = qobject_cast<AbstractPropEditor*>(propClass);
        } else if (tagRole == DBFILTERXML::FILTER) {
            propEditor = qobject_cast<AbstractPropEditor*>(new PropFilter());
        } else if (tagRole == DBQUANTITYXML::QUANTITY) {
            propEditor = qobject_cast<AbstractPropEditor*>(new PropQuantity());
        } else if (tagRole == DBQUANTITYGROUPXML::QUANTITYGROUP) {
            propEditor = qobject_cast<AbstractPropEditor*>(new PropQuantityGroup());
        } else if (tagRole == DBLOVXML::LOV) {
            propEditor = qobject_cast<AbstractPropEditor*>(new PropLov());
        } else if (tagRole == DBNUMERATORXML::NUMERATOR) {
            propEditor = qobject_cast<AbstractPropEditor*>(new PropNumerator());
        } else if (tagRole == DBREFGROUPXML::REFGROUP) {
            propEditor = qobject_cast<AbstractPropEditor*>(new PropRefGroup());
        } else if (tagRole == DBREFXML::REF) {
            propEditor = qobject_cast<AbstractPropEditor*>(new PropRef());
        } else if (tagRole == DBROLEXML::ROLE) {
            propEditor = qobject_cast<AbstractPropEditor*>(new PropRole());
        } else return;

        propEditor->setModel(m_model);
        mainWindow->addSubWindow(propEditor);
        propEditor->setTabName(indexSource);
        propEditor->setCurrent(indexSource);

    } else
        propEditor = qobject_cast<AbstractPropEditor*>(subWindow->widget());

    propEditor->setCurrent(indexSource);
    if (editable) propEditor->edit(true);
}

void ModelerIDEPlug::showPropEditor(const QModelIndex &indexSource)
{
    showPropEditor(indexSource, false);
}

void ModelerIDEPlug::editPropEditor(const QModelIndex &indexSource)
{
    showPropEditor(indexSource, true);
}

void ModelerIDEPlug::closePropEditor(const QModelIndex &index)
{
    PluginManager *pluginManager = PluginManager::instance();
    IMainWindow *mainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    QString subWindowName = this->dataId(index);
    QString tagRole = index.data(TreeXmlModel::TagRole).toString();

    if (tagRole == DBCLASSXML::CLASS) {
        subWindowName = "PropClass::" + subWindowName;
    } else if (tagRole == DBCOMPXML::COMP) {
        subWindowName = "PropComposition::" + subWindowName;
    } else if (tagRole == DBFILTERXML::FILTER) {
        subWindowName = "PropFilter::" + subWindowName;
    } else if (tagRole == DBQUANTITYXML::QUANTITY) {
        subWindowName = "PropEntiry::" + subWindowName;
    } else if (tagRole == DBQUANTITYGROUPXML::QUANTITYGROUP) {
        subWindowName = "PropQuantityGroup::" + subWindowName;
    } else if (tagRole==DBLOVXML::LOV) {
        subWindowName = "PropLov::" + subWindowName;
    } else if (tagRole==DBNUMERATORXML::NUMERATOR) {
        subWindowName = "PropNumerator::" + subWindowName;
    } else if (tagRole==DBREFGROUPXML::REFGROUP) {
        subWindowName = "PropRefGroup::" + subWindowName;
    } else if (tagRole==DBREFXML::REF) {
        subWindowName = "PropRef::" + subWindowName;
    } else if (tagRole==DBROLEXML::ROLE) {
        subWindowName = "PropRole::" + subWindowName;
    }

    QMdiSubWindow *subWindow = mainWindow->subWindow(subWindowName);
    if (subWindow)
        subWindow->close();
}

void ModelerIDEPlug::newClassModel()
{
    closeClassModel();
    createClassModel();
}

void ModelerIDEPlug::openClassModel()
{
    m_fileName = QFileDialog::getOpenFileName(NULL, tr("Открытие модели"),
                                                    ".", tr("Фаил модели (*.xml)"));
    if (m_fileName.isEmpty())
        return;

    QFile file(m_fileName);
    if (file.open(QIODevice::ReadOnly)) {
        QDomDocument document;
        if (document.setContent(&file)) {
           createClassModel(document);
        }
        file.close();
    }
    m_actionSaveModel->setDisabled(true);
}

void ModelerIDEPlug::saveClassModel()
{
    if (m_fileName.isEmpty()) {
        saveAsClassModel();
        return;
    }

    QFile file(m_fileName);
    if ( file.open(QIODevice::WriteOnly) )
    {
        QTextStream TextStream(&file);
        QDomDocument doc = m_model->rootItem()->node().toDocument();
        doc.save(TextStream, 0);
        file.close();
    }
    m_actionSaveModel->setDisabled(true);
}

void ModelerIDEPlug::saveAsClassModel()
{
    m_fileName = QFileDialog::getSaveFileName(NULL, tr("Сохранение модели"),
                                                    "", tr("Фаил модели (*.xml)"));
    if (m_fileName.right(4) != ".xml")
        m_fileName += ".xml";
    QFile file(m_fileName);
    if ( file.open(QIODevice::WriteOnly) )
    {
        QTextStream TextStream(&file);
        QDomDocument doc = m_model->rootItem()->node().toDocument();
        doc.save(TextStream, 0);
        file.close();
    }
    m_actionSaveModel->setDisabled(true);
}

void ModelerIDEPlug::publishClassModel(const QModelIndex &index)
{
    qint32 row=0;
    QModelIndex childIndex = m_model->index(row,0,index);
    while (childIndex.isValid())
    {
        if (childIndex.data(TreeXmlModel::TagRole)==DBATTRXML::ATTR){
            // Создание атрибута
            if (m_model->isInherited(childIndex))
                qDebug() << "Унаследованный атрибут:" << childIndex.data().toString();
            else
                qDebug() << "Атрибут:" << childIndex.data().toString();
        } else if (childIndex.data(TreeXmlModel::TagRole)==DBCOMPXML::COMP) {
            // Создание состава
            if (m_model->isInherited(childIndex))
                qDebug() << "Унаследованный состав:" <<childIndex.data().toString();
            else
                qDebug() << "Состав:" <<childIndex.data().toString();
        } else if (childIndex.data(TreeXmlModel::TagRole)==DBCLASSXML::CLASS) {
            // Создание класса
            qDebug() << "Класс:" << childIndex.data().toString();
        }
        publishClassModel(childIndex);
        childIndex = m_model->index(++row,0,index);
    }
}

void ModelerIDEPlug::publishClassModel()
{
    publishClassModel(QModelIndex());
}

void ModelerIDEPlug::closeClassModel()
{   
    PluginManager *pluginManager = PluginManager::instance();
    IMainWindow *mainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));
    foreach (QMdiSubWindow *subWindow, mainWindow->subWindowList())
        if (subWindow->widget()->objectName().indexOf(QRegExp("^PropClass::"))  != -1
         || subWindow->widget()->objectName().indexOf(QRegExp("^PropFilter::"))  != -1
         || subWindow->widget()->objectName().indexOf(QRegExp("^PropQuantityGroup::"))  != -1
         || subWindow->widget()->objectName().indexOf(QRegExp("^PropQuantity::"))  != -1
         || subWindow->widget()->objectName().indexOf(QRegExp("^PropLov::"))  != -1
         || subWindow->widget()->objectName().indexOf(QRegExp("^PropNumerator::"))  != -1
         || subWindow->widget()->objectName().indexOf(QRegExp("^PropRefGroup::"))  != -1
         || subWindow->widget()->objectName().indexOf(QRegExp("^PropRef::"))  != -1
        ) {
            subWindow->close();
        }

    if (m_model){
        delete m_model;
        m_model = NULL;
        m_actionCloseModel->setDisabled(true);
        m_actionSaveModel->setDisabled(true);
        m_actionSaveAsModel->setDisabled(true);
        m_actionPublishModel->setDisabled(true);
    }
}

}}
