#include "classmodelxml.h"

#include "dbxmlstruct.h"
#include "clogging.h"

namespace  RTPTechGroup {
namespace  MetaDataModel {

ClassModelXml::ClassModelXml(QDomNode document, QObject *parent)
    : TreeXmlHashModel(document, parent)
{
    initTagFilters();
    initDisplayedAttrs();
    initInsertTags();
    initHashAttrs();
    initRelations();

    this->refreshHashing();

    if (document.isNull()) initModel();
}

ClassModelXml::~ClassModelXml()
{

}

void ClassModelXml::initTagFilters()
{
    this->addTagFilter(DBCLASSXML::CLASS);
    this->addTagFilter(DBATTRXML::ATTR);
    this->addTagFilter(DBCOMPXML::COMP);
    this->addTagFilter(DBFILTERXML::FILTER);
    this->addTagFilter(DBFILTERBLOCKXML::BLOCK);
    this->addTagFilter(DBCONDITIONXML::COND);
    this->addTagFilter(DBQUANTITYXML::QUANTITY);
    this->addTagFilter(DBUNITXML::UNIT);
    this->addTagFilter(DBQUANTITYGROUPXML::QUANTITYGROUP);
    this->addTagFilter(DBCLASSLISTXML::CLASSLIST);
    this->addTagFilter(DBQUANTITYLISTXML::QUANTITYLIST);
    this->addTagFilter(DBMODELXML::MODEL);
    this->addTagFilter(DBLOVLISTXML::LOVLIST);
    this->addTagFilter(DBLOVXML::LOV);
    this->addTagFilter(DBLOVVALUEXML::LOVVALUE);
    this->addTagFilter(DBREFLISTXML::REFLIST);
    this->addTagFilter(DBREFGROUPXML::REFGROUP);
    this->addTagFilter(DBREFXML::REF);
    this->addTagFilter(DBLINKTOCLASSXML::LINKTOCLASS);
    this->addTagFilter(DBLINKTOFILTERXML::LINKTOFILTER);
    this->addTagFilter(DBLINKTOREFXML::LINKTOREF);
    this->addTagFilter(DBROLELISTXML::ROLELIST);
    this->addTagFilter(DBROLEXML::ROLE);
    this->addTagFilter(DBPERMISSIONXML::PERMISSION);
    this->addTagFilter(DBNUMERATORLISTXML::NUMERATORLIST);
    this->addTagFilter(DBNUMERATORXML::NUMERATOR);
    this->addTagFilter(DBNUMERATORLOVXML::NUMERATORLOV);
    this->addTagFilter(DBNUMERATORREGEXXML::NUMERATORREGEX);
}

void ClassModelXml::initDisplayedAttrs()
{
    QStringList propsClass;
    propsClass << DBCLASSXML::NAME      << DBCLASSXML::ALIAS
               << DBCLASSXML::MODE      << DBCLASSXML::TYPE
               << DBCLASSXML::PARENT    << DBCLASSXML::VERCOUNT
               << DBCLASSXML::TEMPLATE  << DBCLASSXML::ID
               << DBCLASSXML::ICON;
    this->addDisplayedAttr(DBCLASSXML::CLASS, propsClass,QIcon(":/class"));

    QStringList propsAttr;
    propsAttr << DBATTRXML::NAME           << DBATTRXML::ALIAS
              << DBATTRXML::TYPE           << DBATTRXML::MAXSTRLEN
              << DBATTRXML::ACCURACY       << DBATTRXML::REFCLASS
              << DBATTRXML::PARENT         << DBATTRXML::REFUNIT
              << DBATTRXML::INITIALVAL     << DBATTRXML::LOWERBOUND
              << DBATTRXML::UPPERBOUND     << DBATTRXML::REFLOV
              << DBATTRXML::REFNUMERATOR   << DBATTRXML::GROUP
              << DBATTRXML::ISNULLALLOWED  << DBATTRXML::ISUNIQUE
              << DBATTRXML::ISCANDIDATEKEY << DBATTRXML::ID;
    this->addDisplayedAttr(DBATTRXML::ATTR,propsAttr, QIcon(":/attribute"));
    this->addAttrTag(DBCLASSXML::CLASS,         DBATTRXML::ATTR);
    this->addAttrTag(DBCLASSLISTXML::CLASSLIST, DBATTRXML::ATTR);

    QStringList propsComposition;
    propsComposition << DBCOMPXML::LINKCLASS         << DBCOMPXML::ALIAS
                     << DBCOMPXML::PARENT            << DBCOMPXML::CLASS
                     << DBCOMPXML::ISVIEW            << DBCOMPXML::ISCOMP
                     << DBCOMPXML::DIRECTDESCRIPTION << DBCOMPXML::INVERSEDESCRIPTION
                     << DBCOMPXML::ID;
    this->addDisplayedAttr(DBCOMPXML::COMP, propsComposition, QIcon(":/composition"));
    this->addAttrTag(DBCLASSXML::CLASS, DBCOMPXML::COMP);

    QStringList propsFilter;
    propsFilter << DBFILTERXML::NAME              << DBFILTERXML::ALIAS
                << DBFILTERXML::PARENT            << DBFILTERXML::CLASS
                << DBFILTERXML::DIRECTDESCRIPTION << DBFILTERXML::INVERSEDESCRIPTION
                << DBFILTERXML::ID;
    this->addDisplayedAttr(DBFILTERXML::FILTER, propsFilter, QIcon(":/filter"));
    this->addAttrTag(DBCLASSXML::CLASS, DBFILTERXML::FILTER);

    QStringList propsFilterBlock;
    propsFilterBlock << DBFILTERBLOCKXML::LINKOF << DBFILTERBLOCKXML::PARENT
                     << DBFILTERBLOCKXML::ID;
    this->addDisplayedAttr(DBFILTERBLOCKXML::BLOCK, propsFilterBlock, QIcon(":/block"));

    QStringList propsCondition;
    propsCondition   << DBCONDITIONXML::FIRSTATTR   << DBCONDITIONXML::OPERATOR
                     << DBCONDITIONXML::SECONDATTR  << DBCONDITIONXML::LINKOF
                     << DBCONDITIONXML::PARENT      << DBCONDITIONXML::ID;
    this->addDisplayedAttr(DBCONDITIONXML::COND, propsCondition, QIcon(":/expression"));

    QStringList propsQuantity;
    propsQuantity << DBQUANTITYXML::NAME    << DBQUANTITYXML::ALIAS
                << DBQUANTITYXML::DIMENSION << DBQUANTITYXML::BASICUNIT
                << DBQUANTITYXML::ID;
    this->addDisplayedAttr(DBQUANTITYXML::QUANTITY, propsQuantity, QIcon(":/quantity"));

    QStringList propsUnit;
    propsUnit << DBUNITXML::NAME           << DBUNITXML::CODE
              << DBUNITXML::COEFF          << DBUNITXML::DELTA
              << DBUNITXML::DESIGNATION    << DBUNITXML::SYMBOL
              << DBUNITXML::INTDESIGNATION << DBUNITXML::INTSYMBOL
              << DBUNITXML::PARENT         << DBUNITXML::ID;
    this->addDisplayedAttr(DBUNITXML::UNIT, propsUnit, QIcon(":/unit"));

    QStringList propsGroup;
    propsGroup << DBQUANTITYGROUPXML::NAME   << DBQUANTITYGROUPXML::ALIAS
               << DBQUANTITYGROUPXML::PARENT << DBQUANTITYGROUPXML::ID;
    this->addDisplayedAttr(DBQUANTITYGROUPXML::QUANTITYGROUP, propsGroup, QIcon(":/quantitygroup"));

    QStringList propsClassList;
    propsClassList << DBCLASSLISTXML::NAME   << DBCLASSLISTXML::ALIAS
                   << DBCLASSLISTXML::PARENT << DBCLASSLISTXML::ID;
    this->addDisplayedAttr(DBCLASSLISTXML::CLASSLIST, propsClassList, QIcon(":/classes"));

    QStringList propsQuantityList;
    propsQuantityList << DBQUANTITYLISTXML::NAME   << DBQUANTITYLISTXML::ALIAS
                    << DBQUANTITYLISTXML::PARENT << DBQUANTITYLISTXML::ID;
    this->addDisplayedAttr(DBQUANTITYLISTXML::QUANTITYLIST, propsQuantityList, QIcon(":/units"));

    QStringList propsLovList;
    propsLovList << DBLOVLISTXML::NAME   << DBLOVLISTXML::ALIAS
                 << DBLOVLISTXML::PARENT << DBLOVLISTXML::ID;
    this->addDisplayedAttr(DBLOVLISTXML::LOVLIST, propsLovList, QIcon(":/lovlist"));

    QStringList propsNumeratorList;
    propsNumeratorList << DBNUMERATORLISTXML::NAME   << DBNUMERATORLISTXML::ALIAS
                 << DBNUMERATORLISTXML::PARENT << DBNUMERATORLISTXML::ID;
    this->addDisplayedAttr(DBNUMERATORLISTXML::NUMERATORLIST, propsNumeratorList,
                           QIcon(":/numeratorlist"));

    QStringList propsNumerator;
    propsNumerator << DBNUMERATORXML::NAME   << DBNUMERATORXML::ALIAS
                   << DBNUMERATORXML::UNIQUE << DBNUMERATORXML::STEP
                   << DBNUMERATORXML::PARENT << DBNUMERATORXML::ID;
    this->addDisplayedAttr(DBNUMERATORXML::NUMERATOR, propsNumerator, QIcon(":/numerator"));

    QStringList propsNumeratorLov;
    propsNumeratorLov << DBNUMERATORLOVXML::REFLOV
                      << DBNUMERATORLOVXML::PARENT
                      << DBNUMERATORLOVXML::ID;
    this->addDisplayedAttr(DBNUMERATORLOVXML::NUMERATORLOV,
                           propsNumeratorLov, QIcon(":/lov"));

    QStringList propsNumeratorRegEx;
    propsNumeratorRegEx << DBNUMERATORREGEXXML::REGEX
                        << DBNUMERATORREGEXXML::PARENT
                        << DBNUMERATORREGEXXML::ID;
    this->addDisplayedAttr(DBNUMERATORREGEXXML::NUMERATORREGEX,
                           propsNumeratorRegEx, QIcon(":/numeratorregex"));

    QStringList propsRoleList;
    propsRoleList << DBROLELISTXML::NAME   << DBROLELISTXML::ALIAS
                  << DBROLELISTXML::PARENT << DBROLELISTXML::ID;
    this->addDisplayedAttr(DBROLELISTXML::ROLELIST, propsRoleList, QIcon(":/rolelist"));

    QStringList propsRefList;
    propsRefList << DBREFLISTXML::NAME   << DBREFLISTXML::ALIAS
                 << DBREFLISTXML::PARENT << DBREFLISTXML::ID;
    this->addDisplayedAttr(DBREFLISTXML::REFLIST, propsRefList, QIcon(":/reflist"));

    QStringList propsModel;
    propsModel << DBMODELXML::NAME   << DBMODELXML::ALIAS
               << DBMODELXML::PARENT << DBMODELXML::ID;
    this->addDisplayedAttr(DBMODELXML::MODEL, propsModel, QIcon(":/model"));

    QStringList propsLov;
    propsLov << DBLOVXML::NAME   << DBLOVXML::ALIAS
             << DBLOVXML::PARENT << DBLOVXML::ID
             << DBLOVXML::TYPE   << DBLOVXML::MAXSTRLEN << DBLOVXML::ACCURACY;
    this->addDisplayedAttr(DBLOVXML::LOV, propsLov, QIcon(":/lov"));

    QStringList propsLovValue;
    propsLovValue << DBLOVVALUEXML::NAME   << DBLOVVALUEXML::VALUE
                  << DBLOVVALUEXML::PARENT << DBLOVVALUEXML::ID;
    this->addDisplayedAttr(DBLOVVALUEXML::LOVVALUE, propsLov, QIcon(":/lovvalue"));

    QStringList propsRefGroup;
    propsRefGroup << DBREFGROUPXML::NAME   << DBREFGROUPXML::ALIAS
                  << DBREFGROUPXML::PARENT << DBREFGROUPXML::ID;
    this->addDisplayedAttr(DBREFGROUPXML::REFGROUP, propsRefGroup, QIcon(":/refgroup"));

    QStringList propsRef;
    propsRef << DBREFXML::NAME   << DBREFXML::ALIAS
             << DBREFXML::PARENT << DBREFXML::ID;
    this->addDisplayedAttr(DBREFXML::REF, propsRef, QIcon(":/reference"));

    QStringList propsLinkToClass;
    propsLinkToClass << DBLINKTOCLASSXML::ALIAS  << DBLINKTOCLASSXML::REFCLASS
                     << DBLINKTOCLASSXML::PARENT << DBLINKTOCLASSXML::ID;
    this->addDisplayedAttr(DBLINKTOCLASSXML::LINKTOCLASS, propsLinkToClass, QIcon(":/class"));

    QStringList propsLinkToFilter;
    propsLinkToFilter << DBLINKTOFILTERXML::ALIAS   << DBLINKTOFILTERXML::REFFILTER
                      << DBLINKTOFILTERXML::PARENT << DBLINKTOFILTERXML::ID;
    this->addDisplayedAttr(DBLINKTOFILTERXML::LINKTOFILTER, propsLinkToFilter, QIcon(":/filter"));

    QStringList propsLinkToRef;
    propsLinkToRef << DBLINKTOREFXML::ALIAS  << DBLINKTOREFXML::REFREF
                    << DBLINKTOREFXML::PARENT << DBLINKTOREFXML::ID;
    this->addDisplayedAttr(DBLINKTOREFXML::LINKTOREF, propsLinkToRef, QIcon(":/reference"));

    QStringList propsRole;
    propsRole << DBROLEXML::NAME   << DBROLEXML::ALIAS
              << DBROLEXML::PARENT << DBROLEXML::ID
              << DBROLEXML::DESCRIPTION;
    this->addDisplayedAttr(DBROLEXML::ROLE, propsRole, QIcon(":/role"));

    QStringList propsPermission;
    propsPermission << DBPERMISSIONXML::ROLE     << DBPERMISSIONXML::ISCREATE
                    << DBPERMISSIONXML::ISREAD   << DBPERMISSIONXML::ISWRITE
                    << DBPERMISSIONXML::ISDELETE << DBPERMISSIONXML::ISBLOCK
                    << DBPERMISSIONXML::PARENT   << DBPERMISSIONXML::ID;
    this->addDisplayedAttr(DBPERMISSIONXML::PERMISSION, propsPermission, QIcon(":/role"));
}

void ClassModelXml::initInsertTags()
{
    QStringList insertTags;

    insertTags << DBATTRXML::ATTR     << DBCLASSXML::CLASS << DBCOMPXML::COMP
               << DBFILTERXML::FILTER << DBPERMISSIONXML::PERMISSION;
    this->addInsertTags(DBCLASSXML::CLASS,insertTags);

    insertTags.clear();
    insertTags << DBPERMISSIONXML::PERMISSION;
    this->addInsertTags(DBATTRXML::ATTR, insertTags);

    insertTags.clear();
    insertTags << DBATTRXML::ATTR;
    this->addInsertTags(DBCOMPXML::COMP,insertTags);

    insertTags.clear();
    insertTags << DBFILTERBLOCKXML::BLOCK << DBCONDITIONXML::COND;
    this->addInsertTags(DBFILTERXML::FILTER,insertTags);

    insertTags.clear();
    insertTags << DBCONDITIONXML::COND << DBFILTERBLOCKXML::BLOCK;
    this->addInsertTags(DBFILTERBLOCKXML::BLOCK,insertTags);

    insertTags.clear();
    insertTags << DBATTRXML::ATTR  << DBCLASSXML::CLASS;
    this->addInsertTags(DBCLASSLISTXML::CLASSLIST,insertTags);

    insertTags.clear();
    insertTags << DBQUANTITYGROUPXML::QUANTITYGROUP;
    this->addInsertTags(DBQUANTITYLISTXML::QUANTITYLIST,insertTags);

    insertTags.clear();
    insertTags << DBUNITXML::UNIT;
    this->addInsertTags(DBQUANTITYXML::QUANTITY,insertTags);

    insertTags.clear();
    insertTags << DBCLASSLISTXML::CLASSLIST << DBQUANTITYLISTXML::QUANTITYLIST
               << DBLOVLISTXML::LOVLIST     << DBREFLISTXML::REFLIST
               << DBROLELISTXML::ROLELIST   << DBNUMERATORLISTXML::NUMERATORLIST;
    this->addInsertTags(DBMODELXML::MODEL,insertTags);

    insertTags.clear();
    insertTags << DBQUANTITYXML::QUANTITY;
    this->addInsertTags(DBQUANTITYGROUPXML::QUANTITYGROUP,insertTags);

    insertTags.clear();
    insertTags << DBNUMERATORXML::NUMERATOR;
    this->addInsertTags(DBNUMERATORLISTXML::NUMERATORLIST,insertTags);

    insertTags.clear();
    insertTags << DBNUMERATORLOVXML::NUMERATORLOV << DBNUMERATORREGEXXML::NUMERATORREGEX;
    this->addInsertTags(DBNUMERATORXML::NUMERATOR,insertTags);

    insertTags.clear();
    insertTags << DBLOVXML::LOV;
    this->addInsertTags(DBLOVLISTXML::LOVLIST,insertTags);

    insertTags.clear();
    insertTags << DBLOVVALUEXML::LOVVALUE;
    this->addInsertTags(DBLOVXML::LOV,insertTags);

    insertTags.clear();
    insertTags << DBREFGROUPXML::REFGROUP;
    this->addInsertTags(DBREFLISTXML::REFLIST,insertTags);

    insertTags.clear();
    insertTags << DBREFXML::REF;
    this->addInsertTags(DBREFGROUPXML::REFGROUP,insertTags);

    insertTags.clear();
    insertTags << DBLINKTOCLASSXML::LINKTOCLASS << DBLINKTOREFXML::LINKTOREF;
    this->addInsertTags(DBREFXML::REF,insertTags);

    insertTags.clear();
    insertTags << DBLINKTOFILTERXML::LINKTOFILTER;
    this->addInsertTags(DBLINKTOCLASSXML::LINKTOCLASS,insertTags);

    insertTags.clear();
    insertTags << DBLINKTOCLASSXML::LINKTOCLASS << DBLINKTOREFXML::LINKTOREF;
    this->addInsertTags(DBLINKTOFILTERXML::LINKTOFILTER,insertTags);

    insertTags.clear();
    insertTags << DBROLEXML::ROLE;
    this->addInsertTags(DBROLELISTXML::ROLELIST,insertTags);
}

void ClassModelXml::initHashAttrs()
{
    this->addHashAttr(DBCLASSXML::CLASS,
                      DBCLASSXML::NAME,
                      TreeXmlHashModel::UniqueUpperRename);
    this->addHashAttr(DBATTRXML::ATTR,
                      DBATTRXML::NAME,
                      TreeXmlHashModel::UniqueParentUpperRename);
    this->addHashAttr(DBATTRXML::ATTR,
                      DBATTRXML::REFCLASS,
                      TreeXmlHashModel::NoUnique);
    this->addHashAttr(DBATTRXML::ATTR,
                      DBATTRXML::REFUNIT,
                      TreeXmlHashModel::NoUnique);
    this->addHashAttr(DBATTRXML::ATTR,
                      DBATTRXML::REFLOV,
                      TreeXmlHashModel::NoUnique);
    this->addHashAttr(DBATTRXML::ATTR,
                      DBATTRXML::REFNUMERATOR,
                      TreeXmlHashModel::NoUnique);
    this->addHashAttr(DBCOMPXML::COMP,
                      DBCOMPXML::CLASS,
                      TreeXmlHashModel::NoUnique);
    this->addHashAttr(DBCOMPXML::COMP,
                      DBCOMPXML::LINKCLASS,
                      TreeXmlHashModel::UniqueParent);
    this->addHashAttr(DBFILTERXML::FILTER,
                      DBFILTERXML::CLASS,
                      TreeXmlHashModel::NoUnique);
    this->addHashAttr(DBFILTERXML::FILTER,
                      DBFILTERXML::NAME,
                      TreeXmlHashModel::UniqueParentUpperRename);
    this->addHashAttr(DBQUANTITYXML::QUANTITY,
                      DBQUANTITYXML::NAME,
                      TreeXmlHashModel::UniqueUpperRename);
    this->addHashAttr(DBUNITXML::UNIT,
                      DBUNITXML::NAME,
                      TreeXmlHashModel::UniqueUpperRename);
    this->addHashAttr(DBQUANTITYGROUPXML::QUANTITYGROUP,
                      DBQUANTITYGROUPXML::NAME,
                      TreeXmlHashModel::UniqueUpperRename);
    this->addHashAttr(DBLOVXML::LOV,
                      DBLOVXML::NAME,
                      TreeXmlHashModel::UniqueUpperRename);
    this->addHashAttr(DBLOVVALUEXML::LOVVALUE,
                      DBLOVVALUEXML::NAME,
                      TreeXmlHashModel::UniqueUpperRename);
    this->addHashAttr(DBREFGROUPXML::REFGROUP,
                      DBREFGROUPXML::NAME,
                      TreeXmlHashModel::UniqueUpperRename);
    this->addHashAttr(DBREFXML::REF,
                      DBREFXML::NAME,
                      TreeXmlHashModel::UniqueUpperRename);
    this->addHashAttr(DBROLEXML::ROLE,
                      DBROLEXML::NAME,
                      TreeXmlHashModel::UniqueUpperRename);
    this->addHashAttr(DBPERMISSIONXML::PERMISSION,
                      DBPERMISSIONXML::ROLE,
                      TreeXmlHashModel::UniqueParent);
    this->addHashAttr(DBNUMERATORXML::NUMERATOR,
                      DBNUMERATORXML::NAME,
                      TreeXmlHashModel::UniqueUpperRename);


    this->addHashAttr(DBCLASSXML::CLASS,
                      DBCLASSXML::ID,
                      TreeXmlHashModel::Uuid);
    this->addHashAttr(DBATTRXML::ATTR,
                      DBATTRXML::ID,
                      TreeXmlHashModel::Uuid);
    this->addHashAttr(DBCOMPXML::COMP,
                      DBCOMPXML::ID,
                      TreeXmlHashModel::Uuid);
    this->addHashAttr(DBFILTERXML::FILTER,
                      DBFILTERXML::ID,
                      TreeXmlHashModel::Uuid);
    this->addHashAttr(DBFILTERBLOCKXML::BLOCK,
                      DBFILTERBLOCKXML::ID,
                      TreeXmlHashModel::Uuid);
    this->addHashAttr(DBCONDITIONXML::COND,
                      DBCONDITIONXML::ID,
                      TreeXmlHashModel::Uuid);
    this->addHashAttr(DBQUANTITYXML::QUANTITY,
                      DBQUANTITYXML::ID,
                      TreeXmlHashModel::Uuid);
    this->addHashAttr(DBUNITXML::UNIT,
                      DBUNITXML::ID,
                      TreeXmlHashModel::Uuid);
    this->addHashAttr(DBQUANTITYGROUPXML::QUANTITYGROUP,
                      DBQUANTITYGROUPXML::ID,
                      TreeXmlHashModel::Uuid);
    this->addHashAttr(DBNUMERATORXML::NUMERATOR,
                      DBNUMERATORXML::ID,
                      TreeXmlHashModel::Uuid);
    this->addHashAttr(DBNUMERATORLOVXML::NUMERATORLOV,
                      DBNUMERATORLOVXML::ID,
                      TreeXmlHashModel::Uuid);
    this->addHashAttr(DBNUMERATORREGEXXML::NUMERATORREGEX,
                      DBNUMERATORREGEXXML::ID,
                      TreeXmlHashModel::Uuid);
    this->addHashAttr(DBLOVXML::LOV,
                      DBLOVXML::ID,
                      TreeXmlHashModel::Uuid);
    this->addHashAttr(DBLOVVALUEXML::LOVVALUE,
                      DBLOVVALUEXML::ID,
                      TreeXmlHashModel::Uuid);
    this->addHashAttr(DBREFGROUPXML::REFGROUP,
                      DBREFGROUPXML::ID,
                      TreeXmlHashModel::Uuid);
    this->addHashAttr(DBREFXML::REF,
                      DBREFXML::ID,
                      TreeXmlHashModel::Uuid);
    this->addHashAttr(DBLINKTOCLASSXML::LINKTOCLASS,
                      DBLINKTOCLASSXML::ID,
                      TreeXmlHashModel::Uuid);
    this->addHashAttr(DBLINKTOFILTERXML::LINKTOFILTER,
                      DBLINKTOFILTERXML::ID,
                      TreeXmlHashModel::Uuid);
    this->addHashAttr(DBLINKTOREFXML::LINKTOREF,
                      DBLINKTOREFXML::ID,
                      TreeXmlHashModel::Uuid);
    this->addHashAttr(DBCLASSLISTXML::CLASSLIST,
                      DBCLASSLISTXML::ID,
                      TreeXmlHashModel::Uuid);
    this->addHashAttr(DBROLEXML::ROLE,
                      DBROLEXML::ID,
                      TreeXmlHashModel::Uuid);
    this->addHashAttr(DBPERMISSIONXML::PERMISSION,
                      DBPERMISSIONXML::ID,
                      TreeXmlHashModel::Uuid);
}

void ClassModelXml::initRelations()
{
    this->addRelation(DBATTRXML::ATTR,DBATTRXML::PARENT,
                      DBCLASSLISTXML::CLASSLIST, DBCLASSLISTXML::NAME);
    this->addRelation(DBATTRXML::ATTR,DBATTRXML::REFCLASS,
                      DBCLASSXML::CLASS, DBCLASSXML::NAME);
    this->addRelation(DBATTRXML::ATTR,DBATTRXML::PARENT,
                      DBCLASSXML::CLASS, DBCLASSXML::NAME);
    this->addRelation(DBATTRXML::ATTR,DBATTRXML::PARENT,
                      DBCOMPXML::COMP, DBCOMPXML::NAME);
    this->addRelation(DBATTRXML::ATTR,DBATTRXML::REFUNIT,
                      DBUNITXML::UNIT, DBUNITXML::NAME);
    this->addRelation(DBATTRXML::ATTR,DBATTRXML::REFLOV,
                      DBLOVXML::LOV, DBLOVXML::NAME);
    this->addRelation(DBATTRXML::ATTR,DBATTRXML::REFNUMERATOR,
                      DBNUMERATORXML::NUMERATOR, DBNUMERATORXML::NAME);

    this->addRelation(DBCLASSXML::CLASS, DBCLASSXML::PARENT,
                      DBCLASSXML::CLASS, DBCLASSXML::NAME);
    this->addRelation(DBCLASSXML::CLASS, DBCLASSXML::PARENT,
                      DBCLASSLISTXML::CLASSLIST, DBCLASSLISTXML::NAME);

    this->addRelation(DBCOMPXML::COMP, DBCOMPXML::PARENT,
                      DBCLASSXML::CLASS, DBCLASSXML::NAME);
    this->addRelation(DBCOMPXML::COMP, DBCOMPXML::CLASS,
                      DBCLASSXML::CLASS, DBCLASSXML::NAME);
    this->addRelation(DBCOMPXML::COMP, DBCOMPXML::LINKCLASS,
                      DBCLASSXML::CLASS, DBCLASSXML::NAME);

    this->addRelation(DBFILTERXML::FILTER, DBFILTERXML::PARENT,
                      DBCLASSXML::CLASS, DBCLASSXML::NAME);
    this->addRelation(DBFILTERXML::FILTER, DBFILTERXML::CLASS,
                      DBCLASSXML::CLASS, DBCLASSXML::NAME);

    this->addRelation(DBCONDITIONXML::COND, DBCONDITIONXML::FIRSTATTR,
                      DBCOMPXML::COMP, DBCOMPXML::LINKCLASS);
    this->addRelation(DBCONDITIONXML::COND, DBCONDITIONXML::FIRSTATTR,
                      DBATTRXML::ATTR, DBATTRXML::NAME);
    this->addRelation(DBCONDITIONXML::COND, DBCONDITIONXML::SECONDATTR,
                      DBCOMPXML::COMP, DBCOMPXML::LINKCLASS);
    this->addRelation(DBCONDITIONXML::COND, DBCONDITIONXML::SECONDATTR,
                      DBATTRXML::ATTR, DBATTRXML::NAME);

    this->addRelation(DBQUANTITYXML::QUANTITY, DBQUANTITYXML::BASICUNIT,
                      DBUNITXML::UNIT, DBUNITXML::NAME);
    this->addRelation(DBQUANTITYXML::QUANTITY, DBQUANTITYXML::PARENT,
                      DBQUANTITYGROUPXML::QUANTITYGROUP, DBQUANTITYGROUPXML::NAME);

    this->addRelation(DBUNITXML::UNIT, DBUNITXML::PARENT,
                      DBQUANTITYXML::QUANTITY, DBQUANTITYXML::NAME);

    this->addRelation(DBLOVVALUEXML::LOVVALUE, DBLOVVALUEXML::PARENT,
                      DBLOVXML::LOV, DBLOVXML::NAME);

    this->addRelation(DBNUMERATORLOVXML::NUMERATORLOV, DBNUMERATORLOVXML::REFLOV,
                      DBLOVXML::LOV, DBLOVXML::NAME);

    this->addRelation(DBREFXML::REF, DBREFXML::PARENT,
                      DBREFGROUPXML::REFGROUP, DBREFGROUPXML::NAME);

    this->addRelation(DBLINKTOCLASSXML::LINKTOCLASS, DBLINKTOCLASSXML::PARENT,
                      DBREFXML::REF, DBREFXML::ALIAS);
    this->addRelation(DBLINKTOCLASSXML::LINKTOCLASS, DBLINKTOCLASSXML::PARENT,
                      DBLINKTOFILTERXML::LINKTOFILTER, DBLINKTOFILTERXML::ALIAS);
    this->addRelation(DBLINKTOCLASSXML::LINKTOCLASS, DBLINKTOCLASSXML::REFCLASS,
                      DBCLASSXML::CLASS, DBCLASSXML::NAME);

    this->addRelation(DBLINKTOFILTERXML::LINKTOFILTER, DBLINKTOFILTERXML::PARENT,
                      DBLINKTOCLASSXML::LINKTOCLASS, DBLINKTOCLASSXML::ALIAS);
    this->addRelation(DBLINKTOFILTERXML::LINKTOFILTER, DBLINKTOFILTERXML::REFFILTER,
                      DBFILTERXML::FILTER, DBFILTERXML::NAME);

    this->addRelation(DBLINKTOREFXML::LINKTOREF, DBLINKTOREFXML::PARENT,
                      DBREFXML::REF, DBREFXML::ALIAS);
    this->addRelation(DBLINKTOREFXML::LINKTOREF, DBLINKTOREFXML::PARENT,
                      DBLINKTOFILTERXML::LINKTOFILTER, DBLINKTOFILTERXML::ALIAS);
    this->addRelation(DBLINKTOREFXML::LINKTOREF, DBLINKTOREFXML::REFREF,
                      DBREFXML::REF, DBREFXML::NAME);

    this->addRelation(DBPERMISSIONXML::PERMISSION, DBPERMISSIONXML::PARENT,
                      DBCLASSXML::CLASS, DBCLASSXML::NAME);
    this->addRelation(DBPERMISSIONXML::PERMISSION, DBPERMISSIONXML::PARENT,
                      DBATTRXML::ATTR, DBATTRXML::NAME);
    this->addRelation(DBPERMISSIONXML::PERMISSION, DBPERMISSIONXML::ROLE,
                      DBROLEXML::ROLE, DBROLEXML::NAME);
}

void ClassModelXml::initModel()
{
    QModelIndex indexSource = this->index(-1,-1);
    QModelIndex lastIndex = this->insertLastRows(0,1,indexSource,DBMODELXML::MODEL);
    if (lastIndex.isValid()){
        qint32 column = this->columnDisplayedAttr(DBMODELXML::MODEL,
                                                  DBMODELXML::NAME);
        this->setData(lastIndex.sibling(lastIndex.row(),column),tr("Model"));
        column = this->columnDisplayedAttr(DBMODELXML::MODEL,
                                              DBMODELXML::ALIAS);
        this->setData(lastIndex.sibling(lastIndex.row(),column),tr("Модель"));

        indexSource = lastIndex;
        QModelIndex lastIndex = TreeXmlHashModel::insertLastRows(0,1,indexSource,DBCLASSLISTXML::CLASSLIST);
        if (lastIndex.isValid()){
            qint32 column = this->columnDisplayedAttr(DBCLASSLISTXML::CLASSLIST,
                                                      DBCLASSLISTXML::NAME);
            this->setData(lastIndex.sibling(lastIndex.row(),column),tr("Classes"));
            column = this->columnDisplayedAttr(DBCLASSLISTXML::CLASSLIST,
                                                  DBCLASSLISTXML::ALIAS);
            this->setData(lastIndex.sibling(lastIndex.row(),column),tr("Классы"));
            lastIndex = TreeXmlHashModel::insertLastRows(0,1,lastIndex,DBATTRXML::ATTR);
            if (lastIndex.isValid()){
                column = this->columnDisplayedAttr(DBATTRXML::ATTR,
                                                   DBATTRXML::NAME);
                this->setData(lastIndex.sibling(lastIndex.row(),column),tr("guid"));
                column = this->columnDisplayedAttr(DBATTRXML::ATTR,
                                                   DBATTRXML::ALIAS);
                this->setData(lastIndex.sibling(lastIndex.row(),column),tr("Идентификатор"));
                column = this->columnDisplayedAttr(DBATTRXML::ATTR,
                                                   DBATTRXML::TYPE);
                this->setData(lastIndex.sibling(lastIndex.row(),column),DBATTRTYPEXML::STRING);
                column = this->columnDisplayedAttr(DBATTRXML::ATTR,
                                                   DBATTRXML::MAXSTRLEN);
                this->setData(lastIndex.sibling(lastIndex.row(),column),36);
                column = this->columnDisplayedAttr(DBATTRXML::ATTR,
                                                   DBATTRXML::ISCANDIDATEKEY);
                this->setData(lastIndex.sibling(lastIndex.row(),column),true);
                column = this->columnDisplayedAttr(DBATTRXML::ATTR,
                                                   DBATTRXML::ISUNIQUE);
                this->setData(lastIndex.sibling(lastIndex.row(),column),true);
            }
        }

        lastIndex = TreeXmlHashModel::insertLastRows(0,1,indexSource,DBQUANTITYLISTXML::QUANTITYLIST);
        if (lastIndex.isValid()){
            qint32 column = this->columnDisplayedAttr(DBQUANTITYLISTXML::QUANTITYLIST,
                                                      DBQUANTITYLISTXML::NAME);
            this->setData(lastIndex.sibling(lastIndex.row(),column), tr("Units"));
            column = this->columnDisplayedAttr(DBQUANTITYLISTXML::QUANTITYLIST,
                                                  DBQUANTITYLISTXML::ALIAS);
            this->setData(lastIndex.sibling(lastIndex.row(),column), tr("Единицы измерения"));
        }

        lastIndex = TreeXmlHashModel::insertLastRows(0,1,indexSource, DBLOVLISTXML::LOVLIST);
        if (lastIndex.isValid()){
            qint32 column = this->columnDisplayedAttr(DBLOVLISTXML::LOVLIST,
                                                      DBLOVLISTXML::NAME);
            this->setData(lastIndex.sibling(lastIndex.row(),column), tr("List of Value"));
            column = this->columnDisplayedAttr(DBLOVLISTXML::LOVLIST,
                                                  DBLOVLISTXML::ALIAS);
            this->setData(lastIndex.sibling(lastIndex.row(),column), tr("Список значений"));
        }

        lastIndex = TreeXmlHashModel::insertLastRows(0,1,indexSource, DBNUMERATORLISTXML::NUMERATORLIST);
        if (lastIndex.isValid()){
            qint32 column = this->columnDisplayedAttr(DBNUMERATORLISTXML::NUMERATORLIST,
                                                   DBNUMERATORLISTXML::NAME);
            this->setData(lastIndex.sibling(lastIndex.row(),column), tr("Numerators"));
            column = this->columnDisplayedAttr(DBNUMERATORLISTXML::NUMERATORLIST,
                                               DBNUMERATORLISTXML::ALIAS);
            this->setData(lastIndex.sibling(lastIndex.row(),column), tr("Нумераторы"));
        }

        lastIndex = TreeXmlHashModel::insertLastRows(0,1,indexSource, DBREFLISTXML::REFLIST);
        if (lastIndex.isValid()){
            qint32 column = this->columnDisplayedAttr(DBREFLISTXML::REFLIST,
                                                      DBREFLISTXML::NAME);
            this->setData(lastIndex.sibling(lastIndex.row(),column), tr("References"));

            column = this->columnDisplayedAttr(DBREFLISTXML::REFLIST,
                                                  DBREFLISTXML::ALIAS);
            this->setData(lastIndex.sibling(lastIndex.row(),column), tr("Справочники"));
        }

        lastIndex = TreeXmlHashModel::insertLastRows(0,1,indexSource, DBROLELISTXML::ROLELIST);
        if (lastIndex.isValid()){
            qint32 column = this->columnDisplayedAttr(DBROLELISTXML::ROLELIST,
                                                      DBROLELISTXML::NAME);
            this->setData(lastIndex.sibling(lastIndex.row(),column), tr("Roles"));
            column = this->columnDisplayedAttr(DBROLELISTXML::ROLELIST,
                                               DBROLELISTXML::ALIAS);
            this->setData(lastIndex.sibling(lastIndex.row(),column), tr("Список ролей"));
        }
    }
}

QModelIndex ClassModelXml::insertLastRows(qint32 row, qint32 count, const QModelIndex &parent, QString tag)
{
    if (parent.isValid()) {
        QString parentTag = parent.data(TreeXmlModel::TagRole).toString();
        if ((parentTag == DBCLASSLISTXML::CLASSLIST && tag == DBATTRXML::ATTR)
                || (parentTag == DBMODELXML::MODEL
                    && (tag == DBCLASSLISTXML::CLASSLIST
                        || tag == DBQUANTITYLISTXML::QUANTITYLIST
                        || tag == DBLOVLISTXML::LOVLIST
                        || tag == DBNUMERATORLISTXML::NUMERATORLIST
                        || tag == DBREFLISTXML::REFLIST
                        || tag == DBROLELISTXML::ROLELIST
                        )
                    )
                )
            return index(-1,-1);
    }

    return TreeXmlHashModel::insertLastRows(row, count, parent, tag);
}

bool ClassModelXml::removeRows(qint32 row, qint32 count, const QModelIndex &parent)
{
    if (parent.isValid()) {
        QString parentTag = parent.data(TreeXmlModel::TagRole).toString();
        for (qint32 i = row; i < row+count; ++i) {
            QModelIndex childIndex = childIdx(i,0,parent);
            QString tag = childIndex.data(TreeXmlModel::TagRole).toString();
            if (childIndex.isValid()) {
                if (tag == DBATTRXML::ATTR
                        && parentTag == DBCLASSLISTXML::CLASSLIST)
                    return false;
                if (parentTag == DBMODELXML::MODEL
                        && (tag == DBCLASSLISTXML::CLASSLIST
                            || tag == DBQUANTITYLISTXML::QUANTITYLIST
                            || tag == DBLOVLISTXML::LOVLIST
                            || tag == DBNUMERATORLISTXML::NUMERATORLIST
                            || tag == DBREFLISTXML::REFLIST
                            || tag == DBROLELISTXML::ROLELIST)
                  ) return false;
            }
        }
    }

    return TreeXmlHashModel::removeRows(row, count, parent);
}

bool ClassModelXml::isRemove(const QModelIndex &srcIndex)
{
    QString tag = srcIndex.data(TreeXmlModel::TagRole).toString();
    if (tag == DBMODELXML::MODEL
            || tag == DBCLASSLISTXML::CLASSLIST
            || tag == DBQUANTITYLISTXML::QUANTITYLIST
            || tag == DBNUMERATORLISTXML::NUMERATORLIST
            || tag == DBLOVLISTXML::LOVLIST
            || tag == DBROLELISTXML::ROLELIST)
    {
        return false;
    }

    const TreeXmlHashModel *model = dynamic_cast<const TreeXmlHashModel *>(srcIndex.model());
    if (!model) return false;

    bool success = true;
    if (tag == DBQUANTITYGROUPXML::QUANTITYGROUP) {
        QStringList tags(QStringList() << DBQUANTITYXML::QUANTITY);
        if (model->hasChildren(srcIndex, tags)) {
            m_errorText += tr("Необходимо удалить величины.");
            success = false;
        }
    } else if (tag == DBQUANTITYXML::QUANTITY) {
        QStringList tags(QStringList() << DBUNITXML::UNIT);
        if (model->hasChildren(srcIndex, tags)) {
            m_errorText += tr("Необходимо удалить ЕИ.");
            success = false;
        }
    } else if (tag == DBCLASSXML::CLASS) {
        QStringList tags(QStringList() << DBCLASSXML::CLASS);
        if (model->hasChildren(srcIndex,tags)) {
            m_errorText += tr("Необходимо удалить классы-потомки.");
            if (success)
                success = false;
        }
    } else if (tag == DBLOVXML::LOV) {
       QStringList tags(QStringList() << DBCLASSXML::CLASS);
       if (model->hasChildren(srcIndex, tags)) {
           m_errorText += tr("Необходимо удалить значение списка.");
           success = false;
       }
    } else if (tag == DBREFGROUPXML::REFGROUP) {
        QStringList tags(QStringList() << DBREFXML::REF);
        if (model->hasChildren(srcIndex,tags)) {
            m_errorText += tr("Необходимо удалить справочник.");
            success = false;
        }
    } else if (tag == DBREFXML::REF) {
        QStringList tags(QStringList() << DBLINKTOCLASSXML::LINKTOCLASS
                                       << DBLINKTOFILTERXML::LINKTOFILTER);
        if (model->hasChildren(srcIndex,tags)) {
            m_errorText += tr("Необходимо удалить элементы справочника.");
            success = false;
        }
    } else {
        QString fieldId = model->uuidAttr(tag);
        if (fieldId.isEmpty())
            return true;

        QString guid = srcIndex.sibling(srcIndex.row(),
                                        model->columnDisplayedAttr(tag,fieldId)
                                        ).data().toString();

        for (const TreeXmlHashModel::TagWithAttr &tagWithAttr : model->fromRelation(tag))
        {
            qint32 number = 0;
            QModelIndex linkIndex = model->indexHashAttr(
                        tagWithAttr.tag,
                        tagWithAttr.attr,
                        guid,
                        number
                        );

            while (linkIndex.isValid() && guid!="") {
                QModelIndex linkParent = linkIndex.parent();
                if (linkParent.sibling(linkIndex.parent().row(),0)!= srcIndex){
                    QString parentName;
                    QString name;

                    if (linkParent.data(TreeXmlModel::TagRole) == DBCOMPXML::COMP)
                        parentName = tr(" принадлежащий составу ")
                                + linkParent.sibling(
                                    linkParent.row(),
                                    model->columnDisplayedAttr(
                                        DBCOMPXML::COMP,
                                        DBCOMPXML::NAME)
                                    ).data().toString();
                    else
                        parentName = tr(" принадлежащий классу ")
                                + linkParent.sibling(
                                    linkParent.row(),
                                    model->columnDisplayedAttr(
                                        DBCLASSXML::CLASS,
                                        DBCLASSXML::NAME)
                                    ).data().toString();

                    name = tr("атрибут ")
                            + linkIndex.sibling(linkIndex.row(),
                                                model->columnDisplayedAttr(
                                                    DBATTRXML::ATTR,
                                                    DBATTRXML::NAME)
                                                ).data().toString();

                    m_errorText += QString(tr("Необходимо удалить %1%2.\n\n")).
                            arg(name).arg(parentName);
                    if (success)
                        success = false;
                }
                ++number;
                linkIndex = model->indexHashAttr(
                            tagWithAttr.tag,
                            tagWithAttr.attr,
                            guid,
                            number
                            );
            }
        }
    }

    if (!success) {
        qCWarning(lcMetaDataModel) << lastError();
    }
    return success;
}

QString ClassModelXml::lastError()
{
    return m_errorText;
}

}}
