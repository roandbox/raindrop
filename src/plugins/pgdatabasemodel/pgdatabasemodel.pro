VERSION = 1.0
QMAKE_TARGET_COMPANY = "RTPTechGroup"
QMAKE_TARGET_PRODUCT = "Реализация модели базы данных PostgreSql"
QMAKE_TARGET_DESCRIPTION = "Реализация модели базы данных для PostgreSql"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) RTPTechGroup"

LIBS += -lplugin
include(../plugins.pri)

DEPENDPATH += .
INCLUDEPATH += .
DEFINES +=  PGDATABASEMODEL_LIBRARY
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets sql
}


# Input
HEADERS   = *.h
SOURCES   = *.cpp
RESOURCES = *.qrc