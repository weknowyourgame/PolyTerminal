cmake_minimum_required(VERSION 3.16)

project(QTApplication VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Executable
add_executable(PolyTerminal 
  main.cpp 
  globalShortcuts.cpp 
  globalShortcuts.h
  MainWindow.cpp 
  MainWindow.h
  constants.h)

target_link_libraries(PolyTerminal PRIVATE Qt6::Widgets)
