QT += core gui network widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    chatclient.cpp

HEADERS += \
    inetworkclient.h \
    mainwindow.h \
    chatclient.h

FORMS += \
    mainwindow.ui