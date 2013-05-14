#ifndef PLUG_EXAMPLE_H
#define PLUG_EXAMPLE_H

#include <plugin/iplugin.h>
#include <imainwindow.h>
#include <itreedockwidget.h>
#include "exampleglobal.h"

class EXAMPLELIB ExamplePlug:
        public QObject,
        public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.RTPTechGroup.Raindrop.Example" FILE "example.json")
#endif

public:

    ExamplePlug(QObject *parent = 0);

// IPlugin

    //! Получение экземпляра
    QObject *instance() { return this; }

    //! Получение имени плагина
    QString name() {return tr("Пример плагина");};

    //! Получение иконки плагина
    QIcon icon() {return QIcon(":/example");}

    //! Описание плагина
    QString descript() {return tr("");};

    //! Категория в которой состоит плагин
    QString category() {return tr("");};

    //! Версия плагина
    QString version() {return tr("");};

    //! Производитель плагина
    QString vendor() {return tr("");};

public slots:

    //! Создание закладки
    void createTab();

    //! Создание диалогового окна
    void createDlgWindow();

};

#endif