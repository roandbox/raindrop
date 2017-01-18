import "../main.qbs" as TreeXmlModelProduct

TreeXmlModelProduct {
    name: "tst_treexmlhashmodel"

    productname: ""
    version:     "1.0.0"
    description: ""
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

//    cpp.dynamicLibraries: base.concat(["treexmlmodel"])
    Depends { name: "Qt"; submodules: ["widgets", "xml", "testlib"] }
    Depends { name: "treexmlmodel" }
    Depends { name: "modeltest" }

    files: [
        "tst_treexmlhashmodel.cpp",
        "tst_treexmlhashmodel.qrc"
    ]
}
