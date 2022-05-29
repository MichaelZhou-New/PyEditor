QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(./lsp-cpp/lsp-cpp.pri)

SOURCES += \
    SettingDialog.cpp \
    editor/CodeEdit.cpp \
    editor/LineNumberArea.cpp \
    editor/TabManager.cpp \
    fileBrowser/FileBrowserSortFilterProxyModel.cpp \
    lspClient/LSPClient.cpp \
    main.cpp \
    mainwindow.cpp \
    settings/AppearanceSettingPage.cpp \
    settings/EditorSettingPage.cpp \
    settings/KitsSettingPage.cpp \
    settings/SettingPage.cpp \
    statusBar/CursorInfoWidget.cpp \
    utils/JsonUtil.cpp

HEADERS += \
    SettingDialog.h \
    editor/CodeEdit.h \
    editor/LineNumberArea.h \
    editor/TabManager.h \
    fileBrowser/FileBrowserSortFilterProxyModel.h \
    lspClient/LSPClient.hpp \
    mainwindow.h \
    settings/AppearanceSettingPage.h \
    settings/EditorSettingPage.h \
    settings/KitsSettingPage.h \
    settings/SettingPage.h \
    statusBar/CursorInfoWidget.h \
    utils/JsonUtil.h

FORMS += \
    mainwindow.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    highlighter/highlighter.pri
