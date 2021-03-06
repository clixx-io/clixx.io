TEMPLATE = app
QT += widgets serialport printsupport

HEADERS += mainwindow.h toolbar.h \
    codeeditor.h \
    outputwidget.h \
    projectwidget.h \
    visionwidget.h \
    communicatorserialwidget.h \
    eventplaybackwidget.h \
    hardwarelayoutwidget.h \
    clixxiotprojects.h \
    hardwaregpio.h \
    newhardwareitemdialog.h \
    newconnectionitemdialog.h \
    newgraphicitemdialog.h
SOURCES += mainwindow.cpp toolbar.cpp main.cpp \
    codeeditor.cpp \
    outputwidget.cpp \
    projectwidget.cpp \
    visionwidget.cpp \
    communicatorserialwidget.cpp \
    eventplaybackwidget.cpp \
    hardwarelayoutwidget.cpp \
    clixxiotprojects.cpp \
    hardwaregpio.cpp \
    newhardwareitemdialog.cpp \
    newconnectionitemdialog.cpp \
    newgraphicitemdialog.cpp
build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

RESOURCES += mainwindow.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/mainwindow
INSTALLS += target

FORMS += \
    outputwidget.ui \
    projectwidget.ui \
    visionwidget.ui \
    communicatorserialwidget.ui \
    eventplaybackwidget.ui \
    hardwarelayoutwidget.ui \
    hardwaregpio.ui \
    newhardwareitemdialog.ui \
    newconnectionitemdialog.ui \
    newgraphicitemdialog.ui
