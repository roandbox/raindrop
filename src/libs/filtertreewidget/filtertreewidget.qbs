import "../main.qbs" as LibProduct

LibProduct {
    name: "filtertreewidget"

    product:     "FilterTreeWidget Library"
    version:     "1.0.0"
    description: "FilterTreeWidget Library"
    company:     "RTPTechGroup"
    copyright:   "Copyright (C) RTPTechGroup"

    cpp.defines: base.concat(["FILTERTREEWIDGET_LIBRARY"])
    Depends { name: "Qt"; submodules: ["widgets"] }

    files: [
        "filtertreewidget.h",
        "filtertreewidgetglobal.h",
        "version.h",
        "filtertreewidget.cpp"
    ]
}
