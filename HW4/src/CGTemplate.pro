QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console qt c++11

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += "C:\glew-2.1.0\include"

LIBS += \
	Glu32.lib \
	OpenGL32.lib

SOURCES += \
    main.cpp \
    myglwidget.cpp \
    objUtils.cpp

HEADERS += \
    myglwidget.h \
    objUtils.h