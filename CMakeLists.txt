project(qtxmlrpc)

cmake_minimum_required(VERSION 2.6)

find_package(Qt4 COMPONENTS QtXml QtNetwork QtCore REQUIRED)

add_definitions(${QT_DEFINITIONS})

include(${QT_USE_FILE})
include_directories(
  ${CMAKE_SOURCE_DIR}/utils/src/
  ${CMAKE_SOURCE_DIR}/client/src/
  ${CMAKE_SOURCE_DIR}/server/src/
)

set(QT_USE_XML true)
set(QT_USE_NETWORK true)

add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(utils)
add_subdirectory(xmlrpctest)
