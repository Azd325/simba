# -*- coding: utf-8 -*-
#   Copyright (c) 2010, Tim Kleinschmit.  This file is
#   licensed under the General Public License version 3 or later.
#   See the COPYRIGHT file.
#

QT += webkit sql
TEMPLATE = app
TARGET =
MOC_DIR     = ./moc
OBJECTS_DIR = ./obj
RCC_DIR     = ./rcc 
DEPENDPATH += . src data
INCLUDEPATH += . src

# Input
HEADERS += src/mainwindow.h src/iconloader.h src/database.h
SOURCES += src/main.cpp src/mainwindow.cpp src/iconloader.cpp src/database.cpp 
RESOURCES += data/data.qrc
