
TEMPLATE = app
TARGET = test
INCLUDEPATH += .
CONFIG += debug
CONFIG += c++17

LIBS += -lgtest -lpcosynchro

# Input
SOURCES += main.cpp

HEADERS += \
    abstracttoilets.h \
    toilets.h
