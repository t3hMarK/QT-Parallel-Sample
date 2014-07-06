#-------------------------------------------------
#
# Project created by QtCreator 2014-07-06T14:37:14
#
#-------------------------------------------------

QT += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageManipulator
TEMPLATE = app

HEADERS += \
    gui/imagescaling.h \
    manipulator/bilinearscalingmanipulator.h \
    manipulator/image.h \
    manipulator/imagebuilder.h \
    manipulator/imagemanipulator.h

SOURCES += \
    gui/imagescaling.cpp \
    manipulator/bilinearscalingmanipulator.cpp \
    manipulator/image.cpp \
    manipulator/imagebuilder.cpp \
    manipulator/imagemanipulator.cpp \
    main.cpp
