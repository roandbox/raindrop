import "../main.qbs" as PluginProduct

PluginProduct {
    name: "modeleride"

    productname: "Редактор модели данных"
    version:     "1.0.0"
    description: "ModelerIDE Plugin"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.includePaths:     base.concat(['.'])

    Depends { name: "Qt"; submodules: ["widgets", "xml"] }
    Depends { name: "plugin" }
    Depends { name: "mdiextarea" }
    Depends { name: "treexmlmodel" }
    Depends { name: "treecombobox" }
    Depends { name: "dockwidget" }
    Depends { name: "pushbuttonimage" }
    Depends { name: "metadatamodel" }

    files: [
        "abstracteditorwidget.h",
        "abstractitemwidget.h",
        "abstractmodifywidget.h",
        "abstractpropeditor.h",
        "attrgroupproxymodel.h",
        "attrwidget.h",
        "classtreeview.h",
        "classwidget.h",
        "compositionwidget.h",
        "conditiondelegate.h",
        "conditionproxymodel.h",
        "filterpropwidget.h",
        "filterwidget.h",
        "lovdelegate.h",
        "lovvaluewidget.h",
        "lovwidget.h",
        "modelerideplug.h",
        "numeratorrangedelegate.h",
        "numeratorrangewidget.h",
        "numeratorwidget.h",
        "permdelegate.h",
        "permissionproxymodel.h",
        "permissionwidget.h",
        "propclass.h",
        "propfilter.h",
        "proplov.h",
        "propnumerator.h",
        "propquantity.h",
        "propquantitygroup.h",
        "propref.h",
        "proprefgroup.h",
        "proprole.h",
        "quantitygroupwidget.h",
        "quantitywidget.h",
        "refgroupwidget.h",
        "refitemdelegate.h",
        "refitemproxymodel.h",
        "refitemwidget.h",
        "refproxymodel.h",
        "refwidget.h",
        "regexpvalidator.h",
        "regexpvaluevalidator.h",
        "rolewidget.h",
        "treefilterproxymodel.h",
        "unitdelegate.h",
        "unitwidget.h",
        "version.h",
        "xmldelegate.h",
        "abstracteditorwidget.cpp",
        "abstractitemwidget.cpp",
        "abstractmodifywidget.cpp",
        "abstractpropeditor.cpp",
        "attrgroupproxymodel.cpp",
        "attrwidget.cpp",
        "classtreeview.cpp",
        "classwidget.cpp",
        "compositionwidget.cpp",
        "conditiondelegate.cpp",
        "conditionproxymodel.cpp",
        "filterpropwidget.cpp",
        "filterwidget.cpp",
        "lovdelegate.cpp",
        "lovvaluewidget.cpp",
        "lovwidget.cpp",
        "modelerideplug.cpp",
        "numeratorrangedelegate.cpp",
        "numeratorrangewidget.cpp",
        "numeratorwidget.cpp",
        "permdelegate.cpp",
        "permissionproxymodel.cpp",
        "permissionwidget.cpp",
        "propclass.cpp",
        "propfilter.cpp",
        "proplov.cpp",
        "propnumerator.cpp",
        "propquantity.cpp",
        "propquantitygroup.cpp",
        "propref.cpp",
        "proprefgroup.cpp",
        "proprole.cpp",
        "quantitygroupwidget.cpp",
        "quantitywidget.cpp",
        "refgroupwidget.cpp",
        "refitemdelegate.cpp",
        "refitemproxymodel.cpp",
        "refitemwidget.cpp",
        "refproxymodel.cpp",
        "refwidget.cpp",
        "regexpvalidator.cpp",
        "regexpvaluevalidator.cpp",
        "rolewidget.cpp",
        "treefilterproxymodel.cpp",
        "unitdelegate.cpp",
        "unitwidget.cpp",
        "xmldelegate.cpp",
        "attrwidget.ui",
        "classtreeview.ui",
        "classwidget.ui",
        "compositionwidget.ui",
        "filterpropwidget.ui",
        "filterwidget.ui",
        "lovvaluewidget.ui",
        "lovwidget.ui",
        "numeratorrangewidget.ui",
        "numeratorwidget.ui",
        "permissionwidget.ui",
        "propclass.ui",
        "propfilter.ui",
        "proplov.ui",
        "propnumerator.ui",
        "propquantity.ui",
        "propquantitygroup.ui",
        "propref.ui",
        "proprefgroup.ui",
        "proprole.ui",
        "quantitygroupwidget.ui",
        "quantitywidget.ui",
        "refgroupwidget.ui",
        "refitemwidget.ui",
        "refwidget.ui",
        "rolewidget.ui",
        "unitwidget.ui",
        "modeleride.qrc"
    ]
}
