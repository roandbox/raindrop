include(../../auto.pri)
QT += xml
LIBS += -ltreexmlmodel
SOURCES  += tst_modifyproxymodel.cpp ../../modeltest/dynamictreemodel.cpp ../../modeltest/modeltest.cpp
HEADERS  += ../../modeltest/dynamictreemodel.h ../../modeltest/modeltest.h 

RESOURCES += \
    tst_modifyproxymodel.qrc
