cmake_minimum_required( VERSION 2.8 )

PROJECT( simba )
FIND_PACKAGE( Qt4 REQUIRED )
SET( QT_USE_QTWEBKIT TRUE )

INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} -Wall )

SET( simba_cpp 
	src/main.cpp
	src/mainwindow.cpp)
SET( simba_h 
	src/mainwindow.h)
SET( simba_qrc 
	src/data/data.qrc)

QT4_WRAP_CPP(simba_h_moc ${simba_h})
QT4_ADD_RESOURCES(simba_qrc_rcc ${simba_qrc})

ADD_EXECUTABLE( simba 
    ${simba_cpp}
    ${simba_h_moc}
    ${simba_qrc_rcc} )
TARGET_LINK_LIBRARIES( simba ${QT_LIBRARIES} )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )
