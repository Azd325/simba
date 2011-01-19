cmake_minimum_required( VERSION 2.8 )

PROJECT( simba )
FIND_PACKAGE( Qt4 REQUIRED )
SET( QT_USE_QTWEBKIT TRUE )

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif(NOT CMAKE_BUILD_TYPE)

INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} -Wall -Wextra )

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

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY )

add_custom_target(
    uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake )



# Info
message(STATUS "-------- SIMBA ---------" )
message(STATUS "Build Type:     " ${CMAKE_BUILD_TYPE} )
message(STATUS "Qt Version:     " ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH} )
message(STATUS "Prefix:         " ${CMAKE_INSTALL_PREFIX} )
message(STATUS "Bin:            " ${CMAKE_INSTALL_PREFIX}/bin )
message(STATUS "Data:           " ${CMAKE_INSTALL_PREFIX}/share/simba )
message(STATUS "------------------------" )


