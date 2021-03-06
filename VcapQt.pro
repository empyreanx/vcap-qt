#-------------------------------------------------
#
# Project created by QtCreator 2013-11-17T13:49:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vcap-qt
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11 -DVCAP_SUPPORT_JSON -DVCAP_SUPPORT_PNG -DVCAP_SUPPORT_JPEG

LIBS += -L /usr/lib/libv4l2 -L/usr/local/lib -lvcap -lv4l2 -lpng -ljpeg -ljansson

SOURCES += Main.cpp Utils.cpp \
    ControlWrapper.cpp \
    BooleanControl.cpp \
    MenuControl.cpp \
    IntegerControl.cpp \
    ButtonControl.cpp \
    IntegerMenuControl.cpp \
    MainWindow.cpp

HEADERS  += Utils.hpp \
    ControlWrapper.hpp \
    IntegerControl.hpp \
    MenuControl.hpp \
    BooleanControl.hpp \
    ButtonControl.hpp \
    IntegerMenuControl.hpp \
    MainWindow.hpp

FORMS    += \
    MainWindow.ui

