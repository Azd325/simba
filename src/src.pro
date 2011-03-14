QT += webkit sql
CONFIG += warn_on release qt

TARGET = simba
TEMPLATE = app
LANGUAGE = C++
DESTDIR = ../build

HEADERS += \
	mainwindow.h \
	iconloader.h \
	database.h \
    const.h

SOURCES += \
	main.cpp \
	mainwindow.cpp \
	iconloader.cpp \
	database.cpp 

FORMS += \
    ui/mainwindow.ui \
    ui/about.ui \
    ui/license.ui \
    ui/credits.ui

RESOURCES += \
	data/data.qrc

MOC_DIR     = ../build/moc
OBJECTS_DIR = ../build/obj
UI_DIR      = ../build/ui
RCC_DIR     = ../build/rcc

unix:!macx { # installation on Unix-ish platforms
        isEmpty(INSTALL_PREFIX):INSTALL_PREFIX = /usr/local
        isEmpty(BIN_DIR):BIN_DIR = $$INSTALL_PREFIX/bin
        isEmpty(DATA_DIR):DATA_DIR = $$INSTALL_PREFIX/share
        isEmpty(DOCS_DIR):DOCS_DIR = $$DATA_DIR/doc/simba
        isEmpty(ICON_DIR):ICON_DIR = $$DATA_DIR/pixmaps
        isEmpty(MAN_DIR):MAN_DIR = $$DATA_DIR/man/man1
        isEmpty(DESKTOP_DIR):DESKTOP_DIR = $$DATA_DIR/applications

        target.path = $$BIN_DIR
        documentation.files = LICENSE README.md AUTHORS
        documentation.path = $$DOCS_DIR
        icon.files = data/simba.png
        icon.path = $$ICON_DIR
        man.files = data/man/simba.1
        man.path = $$MAN_DIR
        desktop.files = data/simba.desktop
        desktop.path = $$DESKTOP_DIR
        INSTALLS = target documentation icon man desktop
}

macx {
    ICON = data/simba.icns
}
