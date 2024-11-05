QT += core
QT -= gui

CONFIG += c++17

TARGET = increment
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp

LIBS += -lpcosynchro
