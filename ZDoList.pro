#-------------------------------------------------
#
# Project created by QtCreator 2019-10-16T07:28:17
#
#-------------------------------------------------

QT       += core gui
QMAKE_LFLAGS += -no-pie

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ZDoList
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += SOFTWARE_VERSION=\\\"1.0.0\\\"
DEFINES += GIT_VERSION=\\\"$$system(git --no-pager show -s --pretty=format:%h -n 1)\\\"

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    paramdialog.cpp \
    todo.cpp \
    todotag.cpp \
    datedialog.cpp \
    preferencesdialog.cpp

HEADERS += \
        mainwindow.h \
    paramdialog.h \
    todo.h \
    todotag.h \
    global.h \
    datedialog.h \
    preferencesdialog.h

FORMS += \
        mainwindow.ui \
    datedialog.ui \
    paramdialog.ui \
    preferencesdialog.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
