import "../main.qbs" as LibProduct

LibProduct {
    name: "sqlextension"

    productname: "SqlExtension Library"
    version:     "1.0.0"
    description: "SqlExtension Library"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines: base.concat(["SQLEXTENSION_LIBRARY"])
    Depends { name: "Qt"; submodules: ["sql"] }

    files: [
        "sqlextensionglobal.h",
        "threadquery.cpp",
        "threadquery.h",
        "threadqueryprivate.cpp",
        "threadqueryprivate.h",
        "version.h",
    ]
}