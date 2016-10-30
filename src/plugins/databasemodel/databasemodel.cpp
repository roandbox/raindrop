#include "databasemodel.h"

DatabaseModel::DatabaseModel(QObject *parent):
    QObject(parent), IPlugin("IDatabaseModelBuilder")
{

}

DatabaseModel::~DatabaseModel()
{

}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(databasemodel, DatabaseModel)
#endif

