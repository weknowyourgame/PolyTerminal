cmake_minimum_required(VERSION 3.16)
project(PolyTerminal VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
add_executable(PolyTerminal src/main.cpp src/globalShortcuts.cpp)
target_link_libraries(PolyTerminal PRIVATE Qt6::Widgets)
