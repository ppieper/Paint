HEADERS += \
    main_window.h \
    dialog_windows.h
SOURCES += main.cpp \
    main_window.cpp
CONFIG += qt warn_on
CONFIG += debug
QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
