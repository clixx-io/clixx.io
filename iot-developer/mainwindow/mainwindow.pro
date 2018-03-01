TEMPLATE = app
QT += widgets

HEADERS += colorswatch.h mainwindow.h toolbar.h \
    codeeditor.h \
    outputwidget.h \
    projectwidget.h \
    visionwidget.h \
    communicatorserialwidget.h \
    eventplaybackwidget.h \
    hardwarelayoutwidget.h \
    clixxiotprojects.h
SOURCES += colorswatch.cpp mainwindow.cpp toolbar.cpp main.cpp \
    codeeditor.cpp \
    outputwidget.cpp \
    projectwidget.cpp \
    visionwidget.cpp \
    communicatorserialwidget.cpp \
    eventplaybackwidget.cpp \
    hardwarelayoutwidget.cpp \
    clixxiotprojects.cpp
build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

RESOURCES += mainwindow.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/mainwindows/mainwindow
INSTALLS += target

FORMS += \
    outputwidget.ui \
    projectwidget.ui \
    visionwidget.ui \
    communicatorserialwidget.ui \
    eventplaybackwidget.ui \
    hardwarelayoutwidget.ui