HEADERS += \
    main_window.h \
    dialog_windows.h \
    commands.h \
    draw_area.h
SOURCES += main.cpp \
    main_window.cpp \
    commands.cpp \
    dialog_windows.cpp \
    toolbar.cpp \
    draw_area.cpp
CONFIG += qt warn_on
CONFIG += debug
QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
