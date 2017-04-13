VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "RTPTechGroup"
QMAKE_TARGET_PRODUCT = "SqlExtension Library"
QMAKE_TARGET_DESCRIPTION = "SqlExtension Library"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) RTPTechGroup"

include(../libs.pri)

DEFINES += SQLEXTENSION_LIBRARY
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += sql
}
# Input
HEADERS += sqlextensionglobal.h threadquery.h threadqueryprivate.h
SOURCES += threadquery.cpp threadqueryprivate.cpp