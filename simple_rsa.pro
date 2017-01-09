TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS += -std=c99

SOURCES += main.c \
    isPrime.c \
    keyGenerator.c \
    cryption.c

HEADERS += \
    simple_rsa.h

DISTFILES +=
