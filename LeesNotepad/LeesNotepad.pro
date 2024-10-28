QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    searchdialog.cpp

HEADERS += \
    aboutdialog.h \
    mainwindow.h \
    searchdialog.h

FORMS += \
    aboutdialog.ui \
    mainwindow.ui \
    searchdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    icons/about.png \
    icons/autoWrap.png \
    icons/copy.png \
    icons/cut.png \
    icons/editorColor.png \
    icons/exit.png \
    icons/find.png \
    icons/font.png \
    icons/fontBackgroundColor.png \
    icons/fontColor.png \
    icons/lineNumber.png \
    icons/mainIcon.png \
    icons/newFile.png \
    icons/openFile.png \
    icons/paste.png \
    icons/redo.png \
    icons/replase.png \
    icons/save.png \
    icons/saveAs.png \
    icons/selectAll.png \
    icons/statusbar.png \
    icons/toolbar.png \
    icons/undo.png
