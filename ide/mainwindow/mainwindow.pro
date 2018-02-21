TEMPLATE = app
QT += widgets

HEADERS += colorswatch.h mainwindow.h toolbar.h \
    codeeditor.h
SOURCES += colorswatch.cpp mainwindow.cpp toolbar.cpp main.cpp \
    codeeditor.cpp
build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

RESOURCES += mainwindow.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/mainwindows/mainwindow
INSTALLS += target
