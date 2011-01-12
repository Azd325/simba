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

unix:!macx { # installation on Unix-ish platforms
        isEmpty(INSTALL_PREFIX):INSTALL_PREFIX = /usr/local
        isEmpty(BIN_DIR):BIN_DIR = $$INSTALL_PREFIX/bin
        isEmpty(DATA_DIR):DATA_DIR = $$INSTALL_PREFIX/share
        isEmpty(DOCS_DIR):DOCS_DIR = $$DATA_DIR/doc/texworks
        isEmpty(ICON_DIR):ICON_DIR = $$DATA_DIR/pixmaps
        isEmpty(MAN_DIR):MAN_DIR = $$DATA_DIR/man/man1
        isEmpty(DESKTOP_DIR):DESKTOP_DIR = $$DATA_DIR/applications

        target.path = $$BIN_DIR
        documentation.files = COPYING README
        documentation.path = $$DOCS_DIR
        icon.files = res/images/Simba.png
        icon.path = $$ICON_DIR
        man.files = man/simba.1
        man.path = $$MAN_DIR
        desktop.files = simba.desktop
        desktop.path = $$DESKTOP_DIR
        INSTALLS = target documentation icon man desktop
}
