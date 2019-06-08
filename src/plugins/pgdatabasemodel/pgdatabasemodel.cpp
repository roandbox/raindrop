#include "pgdatabasemodel.h"

#include <metadatamodel/dbxmlstruct.h>
#include "clogging.h"

#include <QDebug>
#include <QSqlError>
#include <QObject>
#include <QtSql/QSql>

#include "pgdatabaseclass.h"
#include "pgdatabasehelper.h"

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::SqlExtension;

namespace RTPTechGroup {
namespace DatabaseModel {

PgDatabaseModel::PgDatabaseModel(IDatabasePool *pool): IDatabaseModel (pool)
{

}

// Вставляет описание класса
void insertClass(ThreadQuery *query, const QString &name, const QString &alias,
                 const QIcon icon, const QString &guid, const QString &parent,
                 const QUuid &operId = QUuid::createUuid())
{
    query->prepare(
                "INSERT INTO " + clsTable(DBCLASSXML::CLASS) + " ("
                + tblField(DBCLASSXML::NAME)     + ","
                + tblField(DBCLASSXML::MODE)     + ","
                + tblField(DBCLASSXML::TYPE)     + ","
                + tblField(DBCLASSXML::ALIAS)    + ","
                + tblField(DBCLASSXML::PARENT)   + ","
                + tblField(DBCLASSXML::TEMPLATE) + ","
                + tblField(DBCLASSXML::VERCOUNT) + ","
                + tblField(DBCLASSXML::ICON)     + ","
                + tblField(DBCLASSXML::ID)
                + ") VALUES("
                  ":name, :mode, :type, :alias, :parent, :template, "
                  ":vercount, :icon, :id"
                  ");"
                );
    query->bindValue(":name", name);
    query->bindValue(":mode", DBACCESSMODEXML::SYSTEM);
    query->bindValue(":type", DBCLASSTYPEXML::NORMAL);
    query->bindValue(":alias", alias);
    query->bindValue(":parent", parent);
    query->bindValue(":template", QVariant::String);
    query->bindValue(":vercount", 0);
    query->bindValue(":icon", icon);
    query->bindValue(":id", guid);

    query->execute(operId);
}

// Вставляет описание значения списка значений
void insertLovValue(ThreadQuery *query, const QString &table, const QString &alias,
                    const QString &value, const QUuid &operId = QUuid::createUuid())
{
    query->prepare("INSERT INTO " + lovTable(table) + " ("
                   + tblField(DBLOVVALUEXML::ALIAS) + ","
                   + tblField(DBLOVVALUEXML::VALUE)
                   + ") VALUES(:alias, :value);");
    query->bindValue(":alias", alias);
    query->bindValue(":value", value);

    query->execute(operId);
}

// Вставляет описание списка значений
void insertLov(ThreadQuery *query, const QString &name, const QString &alias,
               const QString &guid, const QUuid &operId = QUuid::createUuid())
{
    query->prepare("INSERT INTO " + clsTable(DBLOVXML::LOV) + " ("
                   + tblField(DBLOVXML::ALIAS)     + ","
                   + tblField(DBLOVXML::NAME)      + ","
                   + tblField(DBLOVXML::TYPE)      + ","
                   + tblField(DBLOVXML::MAXSTRLEN) + ","
                   + tblField(DBLOVXML::ACCURACY)  + ","
                   + tblField(DBLOVXML::ID)
                   + ") VALUES(:alias, :name, :type, 12, 0, :guid);");
    query->bindValue(":alias", alias);
    query->bindValue(":name", name);
    query->bindValue(":type", DBATTRTYPEXML::STRING);
    query->bindValue(":guid", guid);

    query->execute(operId);
}

IDatabaseError PgDatabaseModel::init(IDatabaseSession *session)
{
    IDatabaseError error(session);

    QUuid uuidOper = QUuid::createUuid();
    ThreadQuery *query = autoDoneQuery(uuidOper, error, m_pool);

    // Создание описания списка значений
    query->execute("CREATE TABLE " + clsTable(DBLOVXML::LOV) + " ("
                   + tblField(DBLOVXML::ALIAS)     + " " + fldStr(256) + ","
                   + tblField(DBLOVXML::NAME)      + " " + fldStr(27)  + ","
                   + tblField(DBLOVXML::TYPE)      + " " + fldStr(15)  + ","
                   + tblField(DBLOVXML::MAXSTRLEN) + " " + fldInt()    + ","
                   + tblField(DBLOVXML::ACCURACY)  + " " + fldInt()    + ","
                   + tblField(DBLOVXML::ID)        + " " + fldGuid()   + " PRIMARY KEY"
                                                                         ");");
    // Создание списка значений типов атрибутов
    query->execute("CREATE TABLE " + lovTable(DBATTRTYPEXML::ATTRTYPE) + " ("
                   + tblField(DBLOVVALUEXML::ALIAS)  + " VARCHAR(256),"
                   + tblField(DBLOVVALUEXML::VALUE)  + " CHAR(12) PRIMARY KEY"
    ");");
    insertLov(query, DBATTRTYPEXML::ATTRTYPE, QObject::tr("Тип атрибута"),
              DBSTRUCTGUIDXML::ATTRTYPE);
    insertLovValue(query, DBATTRTYPEXML::ATTRTYPE,
                   DBATTRTYPEXML::BOOLEAN, DBATTRTYPEXML::BOOLEAN);
    insertLovValue(query, DBATTRTYPEXML::ATTRTYPE,
                   DBATTRTYPEXML::BINARY, DBATTRTYPEXML::BINARY);
    insertLovValue(query, DBATTRTYPEXML::ATTRTYPE,
                   DBATTRTYPEXML::CHAR, DBATTRTYPEXML::CHAR);
    insertLovValue(query, DBATTRTYPEXML::ATTRTYPE,
                   DBATTRTYPEXML::DATE, DBATTRTYPEXML::DATE);
    insertLovValue(query, DBATTRTYPEXML::ATTRTYPE,
                   DBATTRTYPEXML::DECIMAL, DBATTRTYPEXML::DECIMAL);
    insertLovValue(query, DBATTRTYPEXML::ATTRTYPE,
                   DBATTRTYPEXML::DOUBLE, DBATTRTYPEXML::DOUBLE);
    insertLovValue(query, DBATTRTYPEXML::ATTRTYPE,
                   DBATTRTYPEXML::INTEGER, DBATTRTYPEXML::INTEGER);
    insertLovValue(query, DBATTRTYPEXML::ATTRTYPE,
                   DBATTRTYPEXML::STRING, DBATTRTYPEXML::STRING);
    insertLovValue(query, DBATTRTYPEXML::ATTRTYPE,
                   DBATTRTYPEXML::REFERENCE, DBATTRTYPEXML::REFERENCE);
    insertLovValue(query, DBATTRTYPEXML::ATTRTYPE,
                   DBATTRTYPEXML::TIME, DBATTRTYPEXML::TIME);
    insertLovValue(query, DBATTRTYPEXML::ATTRTYPE,
                   DBATTRTYPEXML::TIMESHTAMP, DBATTRTYPEXML::TIMESHTAMP);

    // Создание списка значений типов классов
    query->execute("CREATE TABLE " + lovTable(DBCLASSTYPEXML::CLASSTYPE) + " ("
                   + tblField(DBLOVVALUEXML::ALIAS)  + " VARCHAR(256),"
                   + tblField(DBLOVVALUEXML::VALUE)  + " CHAR(12) PRIMARY KEY"
    ");");
    insertLov(query, DBCLASSTYPEXML::CLASSTYPE, QObject::tr("Тип класса"),
              DBSTRUCTGUIDXML::CLASSTYPE);
    insertLovValue(query, DBCLASSTYPEXML::CLASSTYPE,
                   DBCLASSTYPEXML::ABSTRACT, DBCLASSTYPEXML::ABSTRACT);
    insertLovValue(query, DBCLASSTYPEXML::CLASSTYPE,
                   DBCLASSTYPEXML::CONTEXT, DBCLASSTYPEXML::CONTEXT);
    insertLovValue(query, DBCLASSTYPEXML::CLASSTYPE,
                   DBCLASSTYPEXML::EMBEDDED, DBCLASSTYPEXML::EMBEDDED);
    insertLovValue(query, DBCLASSTYPEXML::CLASSTYPE,
                   DBCLASSTYPEXML::NORMAL, DBCLASSTYPEXML::NORMAL);

    // Создание списка значений режимов доступа
    query->execute("CREATE TABLE " + lovTable(DBACCESSMODEXML::ACCESSMODE) + " ("
                   + tblField(DBLOVVALUEXML::ALIAS)  + " VARCHAR(256),"
                   + tblField(DBLOVVALUEXML::VALUE)  + " CHAR(12) PRIMARY KEY"
    ");");
    insertLov(query, DBACCESSMODEXML::ACCESSMODE, QObject::tr("Режим доступа"),
              DBSTRUCTGUIDXML::ACCESSMODE);
    insertLovValue(query, DBACCESSMODEXML::ACCESSMODE,
                   DBACCESSMODEXML::STANDART, DBACCESSMODEXML::STANDART);
    insertLovValue(query, DBACCESSMODEXML::ACCESSMODE,
                   DBACCESSMODEXML::SYSTEM, DBACCESSMODEXML::SYSTEM);
    insertLovValue(query, DBACCESSMODEXML::ACCESSMODE,
                   DBACCESSMODEXML::USER, DBACCESSMODEXML::USER);

    // Создание списка значений уникальности нумераторов
    query->execute("CREATE TABLE " + lovTable(DBUNIQUENUMERATORXML::UNIQUENUMERATOR) + " ("
                   + tblField(DBLOVVALUEXML::ALIAS)  + " VARCHAR(256),"
                   + tblField(DBLOVVALUEXML::VALUE)  + " CHAR(12) PRIMARY KEY"
    ");");
    insertLov(query, DBUNIQUENUMERATORXML::UNIQUENUMERATOR,
              QObject::tr("Уникальность нумератора"),
              DBSTRUCTGUIDXML::UNIQUENUMERATOR);
    insertLovValue(query, DBUNIQUENUMERATORXML::UNIQUENUMERATOR,
                   DBUNIQUENUMERATORXML::CLASS, DBUNIQUENUMERATORXML::CLASS);
    insertLovValue(query, DBUNIQUENUMERATORXML::UNIQUENUMERATOR,
                   DBUNIQUENUMERATORXML::COMP, DBUNIQUENUMERATORXML::COMP);
    insertLovValue(query, DBUNIQUENUMERATORXML::UNIQUENUMERATOR,
                   DBUNIQUENUMERATORXML::OBJECT, DBUNIQUENUMERATORXML::OBJECT);


    // Создание описания классов
    query->execute("CREATE TABLE " + clsTable(DBCLASSXML::CLASS) + " ("
                   + tblField(DBCLASSXML::NAME)     + " " + fldStr(27)        + ","
                   + tblField(DBCLASSXML::MODE)     + " " + fldStr(12, false) +
                   " REFERENCES " + lovTable(DBACCESSMODEXML::ACCESSMODE) + ","
                   + tblField(DBCLASSXML::TYPE)     + " " + fldStr(12, false) +
                   " REFERENCES " + lovTable(DBCLASSTYPEXML::CLASSTYPE)   + ","
                   + tblField(DBCLASSXML::ALIAS)    + " " + fldStr(256)       + ","
                   + tblField(DBCLASSXML::PARENT)   + " " + fldGuid()         +
                   " REFERENCES " + clsTable(DBCLASSXML::CLASS)           + ","
                   + tblField(DBCLASSXML::TEMPLATE) + " " + fldStr(4000)      + ","
                   + tblField(DBCLASSXML::VERCOUNT) + " " + fldInt()          + ","
                   + tblField(DBCLASSXML::ICON)     + " " + fldStr(4000)      + ","
                   + tblField(DBCLASSXML::ID)       + " " + fldGuid() + " PRIMARY KEY"
    ");");
    insertClass(query, DBCLASSXML::CLASS, QObject::tr("Класс"), QIcon(),
                DBSTRUCTGUIDXML::CLASS, QString());
    insertClass(query, DBLOVXML::LOV, QObject::tr("Список значений"), QIcon(),
                DBSTRUCTGUIDXML::LOV, QString());


    // Создание описания групп величин
    query->execute("CREATE TABLE " + clsTable(DBQUANTITYGROUPXML::QUANTITYGROUP) + " ("
                   + tblField(DBQUANTITYGROUPXML::NAME)   + " " + fldStr(27)  + ","
                   + tblField(DBQUANTITYGROUPXML::ALIAS)  + " " + fldStr(256) + ","
                   //      + tblField(DBQUANTITYGROUPXML::PARENT) + " " + fldGuid()   + ","
                   + tblField(DBQUANTITYGROUPXML::ID)     + " " + fldGuid() + " PRIMARY KEY"
    ");");
    insertClass(query, DBQUANTITYGROUPXML::QUANTITYGROUP,
                QObject::tr("Группа величины"), QIcon(),
                DBSTRUCTGUIDXML::QUANTITYGROUP, QString());

    // Создание описания величин
    query->execute("CREATE TABLE " + clsTable(DBQUANTITYXML::QUANTITY) + " ("
                   + tblField(DBQUANTITYXML::ALIAS)     + " " + fldStr(256) + ","
                   + tblField(DBQUANTITYXML::NAME)      + " " + fldStr(256) + ","
                   + tblField(DBQUANTITYXML::DIMENSION) + " " + fldStr(256) + ","
                   + tblField(DBQUANTITYXML::PARENT)    + " " + fldGuid()   +
                   " REFERENCES " + clsTable(DBQUANTITYGROUPXML::QUANTITYGROUP) + ","
                   + tblField(DBQUANTITYXML::ID)        + " " + fldGuid() + " PRIMARY KEY"
    ");");
    insertClass(query, DBQUANTITYXML::QUANTITY, QObject::tr("Величина"),
                QIcon(), DBSTRUCTGUIDXML::QUANTITY, QString());

    // Создание описания единиц измерения
    query->execute("CREATE TABLE " + clsTable(DBUNITXML::UNIT) + " ("
                   + tblField(DBUNITXML::CODE)           + " " + fldInt()    + ","
                   + tblField(DBUNITXML::COEFF)          + " " + fldDbl()    + ","
                   + tblField(DBUNITXML::DELTA)          + " " + fldDbl()    + ","
                   + tblField(DBUNITXML::NAME)           + " " + fldStr(256) + ","
                   + tblField(DBUNITXML::ID)             + " " + fldGuid()   + " PRIMARY KEY,"
                   + tblField(DBUNITXML::DESIGNATION)    + " " + fldStr(32)  + ","
                   + tblField(DBUNITXML::INTDESIGNATION) + " " + fldStr(32)  + ","
                   + tblField(DBUNITXML::SYMBOL)         + " " + fldStr(32)  + ","
                   + tblField(DBUNITXML::INTSYMBOL)      + " " + fldStr(32)  + ","
                   //      + tblField(DBUNITXML::ALIAS)          + " " + fldStr(256) + ","
                   + tblField(DBUNITXML::PARENT)         + " " + fldGuid() +
                   " REFERENCES " + clsTable(DBQUANTITYXML::QUANTITY) +
    ");");
    query->execute("ALTER TABLE " + clsTable(DBQUANTITYXML::QUANTITY) + " ADD COLUMN "
                   + tblField(DBQUANTITYXML::BASICUNIT) + " " + fldGuid() +
                   " REFERENCES " + clsTable(DBUNITXML::UNIT) +
    ";");
    insertClass(query, DBUNITXML::UNIT, QObject::tr("Единица измерения"),
                QIcon(), DBSTRUCTGUIDXML::UNIT, QString());

    // Создание описания нумератора
    query->execute("CREATE TABLE " + clsTable(DBNUMERATORXML::NUMERATOR) + " ("
                   + tblField(DBNUMERATORXML::ALIAS)  + " " + fldStr(256)       + ","
                   + tblField(DBNUMERATORXML::NAME)   + " " + fldStr(27)        + ","
                   + tblField(DBNUMERATORXML::STEP)   + " " + fldInt()          + ","
                   + tblField(DBNUMERATORXML::UNIQUE) + " " + fldStr(12, false) +
                   " REFERENCES " + lovTable(DBUNIQUENUMERATORXML::UNIQUENUMERATOR) + ","
                   + tblField(DBNUMERATORXML::PARENT) + " " + fldGuid()         + ","
                   + tblField(DBNUMERATORXML::ID)     + " " + fldGuid() + " PRIMARY KEY"
    ");");
    insertClass(query, DBNUMERATORXML::NUMERATOR, QObject::tr("Нумератор"),
                QIcon(), DBSTRUCTGUIDXML::NUMERATOR, QString());


    // Создание описания списка значений в нумераторе
    query->execute("CREATE TABLE " + clsTable(DBNUMERATORLOVXML::NUMERATORLOV) + " ("
                   + tblField(DBNUMERATORLOVXML::REFLOV) + " " + fldGuid() +
                   " REFERENCES " + clsTable(DBLOVXML::LOV) + ","
                   + tblField(DBNUMERATORLOVXML::PARENT) + " " + fldGuid() +
                   " REFERENCES " + clsTable(DBNUMERATORXML::NUMERATOR) + ","
                   + tblField(DBNUMERATORLOVXML::ID)     + " " + fldGuid() + " PRIMARY KEY"
    ");");
    insertClass(query, DBNUMERATORLOVXML::NUMERATORLOV,
                QObject::tr("Список значений нумератора"),
                QIcon(), DBSTRUCTGUIDXML::NUMERATORLOV, QString());

    // Создание описания регулярных выражений в нумераторе
    query->execute("CREATE TABLE " + clsTable(DBNUMERATORREGEXXML::NUMERATORREGEX) + " ("
                   + tblField(DBNUMERATORREGEXXML::REGEX)  + " " + fldStr(256) + ","
                   + tblField(DBNUMERATORREGEXXML::PARENT) + " " + fldGuid() +
                   " REFERENCES " + clsTable(DBNUMERATORXML::NUMERATOR) + ","
                   + tblField(DBNUMERATORREGEXXML::ID)     + " " + fldGuid() + " PRIMARY KEY"
    ");");
    insertClass(query, DBNUMERATORREGEXXML::NUMERATORREGEX,
                QObject::tr("Регулярное выражение нумератора"),
                QIcon(), DBSTRUCTGUIDXML::NUMERATORREGEX, QString());

    // Создание описания атрибутов
    query->execute("CREATE TABLE " + clsTable(DBATTRXML::ATTR) + " ("
                   + tblField(DBATTRXML::NAME)           + " " + fldStr(27)   + ","
                   + tblField(DBATTRXML::ALIAS)          + " " + fldStr(256)  + ","
                   + tblField(DBATTRXML::TYPE)           + " " + fldStr(10, false) +
                   " REFERENCES " + lovTable(DBATTRTYPEXML::ATTRTYPE)     + ","
                   + tblField(DBATTRXML::GROUP)          + " " + fldStr(256)  + ","
                   + tblField(DBATTRXML::MAXSTRLEN)      + " " + fldInt()     + ","
                   + tblField(DBATTRXML::ACCURACY)       + " " + fldInt()     + ","
                   + tblField(DBATTRXML::ISNULLALLOWED)  + " " + fldBool()    + ","
                   + tblField(DBATTRXML::ISUNIQUE)       + " " + fldBool()    + ","
                   + tblField(DBATTRXML::ISCANDIDATEKEY) + " " + fldBool()    + ","
                   + tblField(DBATTRXML::INITIALVAL)     + " " + fldStr(4000) + ","
                   + tblField(DBATTRXML::LOWERBOUND)     + " " + fldStr(4000) + ","
                   + tblField(DBATTRXML::UPPERBOUND)     + " " + fldStr(4000) + ","
                   + tblField(DBATTRXML::REFCLASS)       + " " + fldGuid() +
                   " REFERENCES " + clsTable(DBCLASSXML::CLASS) + ","
                   + tblField(DBATTRXML::REFUNIT)        + " " + fldGuid() +
                   " REFERENCES " + clsTable(DBUNITXML::UNIT) + ","
                   + tblField(DBATTRXML::REFLOV)         + " " + fldGuid() +
                   " REFERENCES " + clsTable(DBLOVXML::LOV) + ","
                   + tblField(DBATTRXML::REFNUMERATOR)   + " " + fldGuid() +
                   " REFERENCES " + clsTable(DBNUMERATORXML::NUMERATOR) + ","
                   + tblField(DBATTRXML::PARENT)         + " " + fldGuid() +
                   " REFERENCES " + clsTable(DBCLASSXML::CLASS) + ","
                   + tblField(DBATTRXML::ID)             + " " + fldGuid() + " PRIMARY KEY"
    ");");
    insertClass(query, DBATTRXML::ATTR, QObject::tr("Атрибут"), QIcon(),
                DBSTRUCTGUIDXML::ATTR, QString());

    // Создание описания состава
    query->execute("CREATE TABLE " + clsTable(DBCOMPXML::COMP) + " ("
                   + tblField(DBCOMPXML::LINKCLASS)          + " " + fldGuid() +
                   " REFERENCES " + clsTable(DBCLASSXML::CLASS) + ","
                   + tblField(DBCOMPXML::ALIAS)              + " " + fldStr(256) + ","
                   + tblField(DBCOMPXML::PARENT)             + " " + fldGuid() +
                   " REFERENCES " + clsTable(DBCLASSXML::CLASS) + ","
                   + tblField(DBCOMPXML::CLASS)              + " " + fldGuid()   + ","
                   + tblField(DBCOMPXML::ISVIEW)             + " " + fldBool()   + ","
                   + tblField(DBCOMPXML::DIRECTDESCRIPTION)  + " " + fldStr(256) + ","
                   + tblField(DBCOMPXML::INVERSEDESCRIPTION) + " " + fldStr(256) + ","
                   + tblField(DBCOMPXML::ISCOMP)             + " " + fldBool()   + ","
                   + tblField(DBCOMPXML::ID) + " " + fldGuid() + " PRIMARY KEY"
    ");");
    insertClass(query, DBCOMPXML::COMP, QObject::tr("Состав"), QIcon(),
                DBSTRUCTGUIDXML::COMP, QString());

    // Создание описания фильтра
    query->execute("CREATE TABLE " + clsTable(DBFILTERXML::FILTER) + " ("
                   + tblField(DBFILTERXML::NAME)               + " " + fldStr(27)  + ","
                   + tblField(DBFILTERXML::ALIAS)              + " " + fldStr(256) + ","
                   + tblField(DBFILTERXML::PARENT)             + " " + fldGuid() +
                   " REFERENCES " + clsTable(DBCLASSXML::CLASS) + ","
                   + tblField(DBFILTERXML::CLASS)              + " " + fldGuid() +
                   " REFERENCES " + clsTable(DBCLASSXML::CLASS) + ","
                   + tblField(DBFILTERXML::DIRECTDESCRIPTION)  + " " + fldStr(256) + ","
                   + tblField(DBFILTERXML::INVERSEDESCRIPTION) + " " + fldStr(256) + ","
                   + tblField(DBFILTERXML::ID) + " " + fldGuid() + " PRIMARY KEY"
    ");");
    insertClass(query, DBFILTERXML::FILTER, QObject::tr("Фильтр"), QIcon(),
                DBSTRUCTGUIDXML::FILTER, QString());

    // Создание описания блока условий фильтра
    query->execute("CREATE TABLE " + clsTable(DBFILTERBLOCKXML::BLOCK) + " ("
                   + tblField(DBFILTERBLOCKXML::PARENT) + " " + fldGuid() +
                   " REFERENCES " + clsTable(DBFILTERXML::FILTER) + ","
                   //      + tblField(DBFILTERBLOCKXML::ALIAS)  + " " + fldStr(256) + ","
                   + tblField(DBFILTERBLOCKXML::LINKOF) + " " + fldStr(10)  + ","
                   + tblField(DBFILTERBLOCKXML::ID) + " " + fldGuid() + " PRIMARY KEY"
    ");");
    insertClass(query, DBFILTERBLOCKXML::BLOCK,
                QObject::tr("Блок условий фильтра"), QIcon(),
                DBSTRUCTGUIDXML::BLOCK, QString());

    // Создание описания условия фильтра
    query->execute("CREATE TABLE " + clsTable(DBCONDITIONXML::COND) + " ("
                   + tblField(DBCONDITIONXML::FIRSTATTR)  + " " + fldGuid()  +
                   " REFERENCES " + clsTable(DBATTRXML::ATTR) + ","
                   //      + tblField(DBCONDITIONXML::ALIAS)      + " " + fldStr(256) + ","
                   + tblField(DBCONDITIONXML::PARENT)     + " " + fldGuid()  +
                   " REFERENCES " + clsTable(DBFILTERXML::FILTER) +
                   " REFERENCES " + clsTable(DBFILTERBLOCKXML::BLOCK) + ","
                   + tblField(DBCONDITIONXML::SECONDATTR) + " " + fldGuid()  +
                   " REFERENCES " + clsTable(DBATTRXML::ATTR) + ","
                   + tblField(DBCONDITIONXML::OPERATOR)   + " " + fldStr(10) + ","
                   + tblField(DBCONDITIONXML::LINKOF)     + " " + fldStr(10) + ","
                   + tblField(DBCONDITIONXML::ID) + " " + fldGuid() + " PRIMARY KEY"
    ");");
    insertClass(query, DBCONDITIONXML::COND,
                QObject::tr("Условие фильтра"), QIcon(),
                DBSTRUCTGUIDXML::COND, QString());

    // Создание описания группы справочников
    query->execute("CREATE TABLE " + clsTable(DBREFGROUPXML::REFGROUP) + " ("
                   + tblField(DBREFGROUPXML::ALIAS)  + " " + fldStr(256) + ","
                   + tblField(DBREFGROUPXML::NAME)   + " " + fldStr(27)  + ","
                   + tblField(DBREFGROUPXML::PARENT) + " " + fldGuid()   + ","
                   + tblField(DBREFGROUPXML::ID)     + " " + fldGuid() + " PRIMARY KEY"
    ");");
    insertClass(query, DBREFGROUPXML::REFGROUP,
                QObject::tr("Группа справочников"), QIcon(),
                DBSTRUCTGUIDXML::REFGROUP, QString());

    // Создание описания справочника
    query->execute("CREATE TABLE " + clsTable(DBREFXML::REF) + " ("
                   + tblField(DBREFXML::ALIAS)  + " " + fldStr(256) + ","
                   + tblField(DBREFXML::NAME)   + " " + fldStr(27)  + ","
                   + tblField(DBREFXML::PARENT) + " " + fldGuid()   +
                   " REFERENCES " + clsTable(DBREFGROUPXML::REFGROUP) + ","
                   + tblField(DBREFXML::ID)     + " " + fldGuid() + " PRIMARY KEY"
    ");");
    insertClass(query, DBREFXML::REF,
                QObject::tr("Справочник"), QIcon(),
                DBSTRUCTGUIDXML::REF, QString());

    // Создание описания фильтра справочника
    query->execute("CREATE TABLE " + clsTable(DBLINKTOFILTERXML::LINKTOFILTER) + " ("
                   + tblField(DBLINKTOFILTERXML::ALIAS)     + " " + fldStr(256) + ","
                   + tblField(DBLINKTOFILTERXML::REFFILTER) + " " + fldGuid()   +
                   " REFERENCES " + clsTable(DBFILTERXML::FILTER) + ","
                   + tblField(DBLINKTOFILTERXML::ID) + " " + fldGuid() + " PRIMARY KEY"
    ");");
    insertClass(query, DBLINKTOFILTERXML::LINKTOFILTER,
                QObject::tr("Фильтр справочника"), QIcon(),
                DBSTRUCTGUIDXML::LINKTOFILTER, QString());

    // Создание описания ссылки на справочник
    query->execute("CREATE TABLE " + clsTable(DBLINKTOREFXML::LINKTOREF) + " ("
                   + tblField(DBLINKTOREFXML::ALIAS)  + " " + fldStr(256) + ","
                   + tblField(DBLINKTOREFXML::REFREF) + " " + fldGuid()   +
                   " REFERENCES " + clsTable(DBREFXML::REF) + ","
                   + tblField(DBLINKTOREFXML::PARENT) + " " + fldGuid()   +
                   " REFERENCES " + clsTable(DBLINKTOFILTERXML::LINKTOFILTER) +
                   " REFERENCES " + clsTable(DBREFXML::REF) + ","
                   + tblField(DBLINKTOREFXML::ID) + " " + fldGuid() + " PRIMARY KEY"
    ");");
    insertClass(query, DBLINKTOREFXML::LINKTOREF,
                QObject::tr("Ссылка на справочник"), QIcon(),
                DBSTRUCTGUIDXML::LINKTOREF, QString());

    // Создание описания класса справочника
    query->execute("CREATE TABLE " + clsTable(DBLINKTOCLASSXML::LINKTOCLASS) + " ("
                   + tblField(DBLINKTOCLASSXML::ALIAS)    + " " + fldStr(256) + ","
                   + tblField(DBLINKTOCLASSXML::REFCLASS) + " " + fldGuid()   +
                   " REFERENCES " + clsTable(DBCLASSXML::CLASS) + ","
                   + tblField(DBLINKTOCLASSXML::PARENT)    + " " + fldGuid()  +
                   " REFERENCES " + clsTable(DBLINKTOFILTERXML::LINKTOFILTER) +
                   " REFERENCES " + clsTable(DBREFXML::REF) + ","
                   + tblField(DBLINKTOCLASSXML::ID) + " " + fldGuid() + " PRIMARY KEY"
    ");");
    insertClass(query, DBLINKTOCLASSXML::LINKTOCLASS,
                QObject::tr("Класс справочника"), QIcon(),
                DBSTRUCTGUIDXML::LINKTOCLASS, QString());
    query->execute("ALTER TABLE " + clsTable(DBLINKTOFILTERXML::LINKTOFILTER)
                   + " ADD COLUMN "
                   + tblField(DBLINKTOFILTERXML::PARENT)  + " " + fldGuid()   +
                   " REFERENCES " + clsTable(DBLINKTOCLASSXML::LINKTOCLASS) +
                   " REFERENCES " + clsTable(DBLINKTOREFXML::LINKTOREF) +
    ";");

    // Создание описания роли
    query->execute("CREATE TABLE " + clsTable(DBROLEXML::ROLE) + " ("
                   + tblField(DBROLEXML::ALIAS)       + " " + fldStr(256)  + ","
                   + tblField(DBROLEXML::NAME)        + " " + fldStr(27)   + ","
                   + tblField(DBROLEXML::DESCRIPTION) + " " + fldStr(4000) + ","
                   + tblField(DBROLEXML::PARENT)      + " " + fldGuid()    + ","
                   + tblField(DBROLEXML::ID) + " " + fldGuid() + " PRIMARY KEY"
    ");");
    insertClass(query, DBROLEXML::ROLE,
                QObject::tr("Роль"), QIcon(),
                DBSTRUCTGUIDXML::ROLE, QString());

    // Создание описания прав доступа
    query->execute("CREATE TABLE " + clsTable(DBPERMISSIONXML::PERMISSION) + " ("
                   + tblField(DBPERMISSIONXML::ROLE)     + " " + fldGuid() +
                   " REFERENCES " + clsTable(DBROLEXML::ROLE)   + ","
                   + tblField(DBPERMISSIONXML::ISCREATE) + " " + fldBool() + ","
                   + tblField(DBPERMISSIONXML::ISREAD)   + " " + fldBool() + ","
                   + tblField(DBPERMISSIONXML::ISWRITE)  + " " + fldBool() + ","
                   + tblField(DBPERMISSIONXML::ISDELETE) + " " + fldBool() + ","
                   + tblField(DBPERMISSIONXML::ISBLOCK)  + " " + fldBool() + ","
                   + tblField(DBPERMISSIONXML::PARENT)   + " " + fldGuid() +
                   " REFERENCES " + clsTable(DBCLASSXML::CLASS) +
                   " REFERENCES " + clsTable(DBATTRXML::ATTR)   + ","
                   + tblField(DBPERMISSIONXML::ID) + " " + fldGuid() + " PRIMARY KEY"
    ");");
    insertClass(query, DBPERMISSIONXML::PERMISSION,
                QObject::tr("Право доступа"), QIcon(),
                DBSTRUCTGUIDXML::PERMISSION, QString());

    return error;
}

IDatabaseClass *PgDatabaseModel::oneClass(const QString &name)
{
    return new PgDatabaseClass(name, this->m_pool);
}

IDatabaseClasses *PgDatabaseModel::classList(const QString &baseClass)
{
    Q_UNUSED(baseClass)

    return nullptr;
}

}}
