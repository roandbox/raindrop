######################################################################
# Automatically generated by qmake (2.01a) ?? 25. ??? 15:05:28 2011
######################################################################
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

QT += xml
LIBS += -lplugin -ltreedockwidget -lmdiextarea -ltreexmlmodel -ltreecombobox
include(../plugins.pri)
# Input
HEADERS += modelerideplug.h \
    propclass.h \
    dbxmlstruct.h \
    classtreeview.h \
    treefilterproxymodel.h \
    attrwidget.h \
    classwidget.h \
    compositionwidget.h \
    propcomposition.h \
    compositionpropwidget.h \
    xmldelegate.h
SOURCES += modelerideplug.cpp \
    propclass.cpp \
    classtreeview.cpp \
    treefilterproxymodel.cpp \
    attrwidget.cpp \
    classwidget.cpp \
    compositionwidget.cpp \
    propcomposition.cpp \
    compositionpropwidget.cpp \
    xmldelegate.cpp
RESOURCES += modeleride.qrc

FORMS += \
    propclass.ui \
    classtreeview.ui \
    attrwidget.ui \
    classwidget.ui \
    compositionwidget.ui \
    propcomposition.ui \
    compositionpropwidget.ui
