QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
RC_ICONS = icon.ico
DESTDIR = ./bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    appbasewindow.cpp \
    configmgr.cpp \
    custom/linkdelegate.cpp \
    custom/mysqlquerymodel.cpp \
    dialog/bloggerrevisedialog.cpp \
    dialog/datareviewdialog.cpp \
    dialog/inputbasedialog.cpp \
    dialog/logindialog.cpp \
    dialog/prjinfodialog.cpp \
    dialog/registerdialog.cpp \
    global.cpp \
    main.cpp \
    mainwindow.cpp \
    sqlmgr.cpp \
    widget_projectManage.cpp

HEADERS += \
    appbasewindow.h \
    configmgr.h \
    custom/linkdelegate.h \
    custom/mysqlquerymodel.h \
    dialog/bloggerrevisedialog.h \
    dialog/datareviewdialog.h \
    dialog/inputbasedialog.h \
    dialog/logindialog.h \
    dialog/prjinfodialog.h \
    dialog/registerdialog.h \
    global.h \
    mainwindow.h \
    singleton.h \
    sqlmgr.h \
    widget_projectManage.h

FORMS += \
    appbasewindow.ui \
    dialog/bloggerrevisedialog.ui \
    dialog/datareviewdialog.ui \
    dialog/logindialog.ui \
    dialog/prjinfodialog.ui \
    dialog/registerdialog.ui \
    mainwindow.ui \
    widget_projectManage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rc.qrc

DISTFILES += \
    config.ini

win32:CONFIG(debug, debug | release)
{
    #指定要拷贝的文件目录为工程目录下release目录下的所有dll、lib文件，例如工程目录在D:\QT\Test
    #PWD就为D:/QT/Test，DllFile = D:/QT/Test/release/*.dll
    TargetConfig = $${PWD}/config.ini
    #将输入目录中的"/"替换为"\"
    TargetConfig = $$replace(TargetConfig, /, \\)
    #将输出目录中的"/"替换为"\"
    OutputDir =  $${OUT_PWD}/$${DESTDIR}
    OutputDir = $$replace(OutputDir, /, \\)
    //执行copy命令
    QMAKE_POST_LINK += copy /Y \"$$TargetConfig\" \"$$OutputDir\"
}
