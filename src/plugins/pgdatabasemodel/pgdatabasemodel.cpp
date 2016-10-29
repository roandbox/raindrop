#include "pgdatabasemodel.h"

#include "pgdatabaseattr.h"
#include "pgdatabasecomposition.h"

PgDatabaseModel::PgDatabaseModel(QObject *parent):
    QObject(parent), IPlugin("")
{
    IDatabaseAttr *m_attr = new PgDatabaseAttr();
    IDatabaseComposition *m_comp = new PgDatabaseComposition();
    QVariant x(10);
    IDatabaseExpression e1 = (*m_attr == *m_attr);
    IDatabaseExpression e2 = (*m_attr == 10.5);
    e1 = (*m_comp == *m_attr);
    e1 = (*m_comp == *m_comp);
    e2 = (*m_attr == *m_comp);
    IDatabaseExpression *e3 = new IDatabaseExpression();
    *e3 = (e1 && e2);
    *e3 = ((*m_attr == *m_attr) && (*m_attr == 10.5));
    *e3 = (e1 && e2);
    delete m_attr;
    delete m_comp;
    delete e3;
}

PgDatabaseModel::~PgDatabaseModel()
{

}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(pgdatabasemodel, PgDatabaseModel)
#endif


