HEADERS += my_widget.h
SOURCES += my_widget.cpp main.cpp
CONFIG += qt warn_on
CONFIG += debug
QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
