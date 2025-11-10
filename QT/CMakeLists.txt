cmake_minimum_required(VERSION 3.21)

project(QTApplication VERSION 0.1)

# Set default build type if not specified (for single-config generators like Ninja, Makefiles)
if("${CMAKE_GENERATOR}" MATCHES "Ninja|Unix Makefiles" AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Fetch httplib
include(FetchContent)
FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG        v0.15.3
)
FetchContent_MakeAvailable(httplib)

# CEF_ROOT setup
set(CEF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/third_party/cef_binary_141.0.11+g7e73ac4+chromium-141.0.7390.123_linux64")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CEF_ROOT}/cmake")

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Load CEF configuration
find_package(CEF REQUIRED)

# Include the libcef_dll_wrapper target
add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} libcef_dll_wrapper)

# CEF App Library - Build CEF app components as a static library
add_library(cef_app_lib STATIC
  cef_app/app.cpp
  cef_app/app.h
  cef_app/client.cpp
  cef_app/client.h
)

# Set include directories for CEF app
target_include_directories(cef_app_lib PRIVATE
  ${CEF_ROOT}
  ${CMAKE_CURRENT_SOURCE_DIR}/cef_app
)

# Apply CEF target properties (this sets CEF compiler flags, defines, etc.)
SET_LIBRARY_TARGET_PROPERTIES(cef_app_lib)

# Link CEF app library against CEF
target_link_libraries(cef_app_lib PRIVATE
  libcef_dll_wrapper
)

# Add logical target for libcef library (for Linux)
if(OS_LINUX)
  ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")
  target_link_libraries(cef_app_lib PRIVATE libcef_lib ${CEF_STANDARD_LIBS})
endif()

# Disable sandbox
if(NOT DEFINED USE_SANDBOX)
  set(USE_SANDBOX OFF)
endif()

# Note: CEF Standalone Executable removed - using CEF as library only
# CEF components (app.cpp, client.cpp) are available via cef_app_lib
# for embedding in Qt application

# Main Qt Executable
add_executable(PolyTerminal
  main.cpp
  globalShortcuts.cpp
  globalShortcuts.h
  MainWindow.cpp
  MainWindow.h
  constants.h
  pages/auth.cpp
  pages/dashboard.cpp
  pages/portfolio.cpp
  pages/event.cpp
  tabs/trendingTab.cpp
  tabs/newsTab.cpp
  tabs/politicsTab.cpp
  tabs/worldTab.cpp
  tabs/sportsTab.cpp
  tabs/techTab.cpp
  component/card.cpp)

target_link_libraries(PolyTerminal PRIVATE 
  Qt6::Widgets
  cef_app_lib
  httplib::httplib
)

# Include CEF headers for Qt app if needed
target_include_directories(PolyTerminal PRIVATE
  ${CEF_ROOT}
  ${CMAKE_CURRENT_SOURCE_DIR}/cef_app
)

# Link CEF libraries to Qt app since we're using CEF directly
if(OS_LINUX)
  target_link_libraries(PolyTerminal PRIVATE 
    libcef_lib 
    ${CEF_STANDARD_LIBS}
    libcef_dll_wrapper
  )
  
  # Set rpath for CEF libraries
  set_target_properties(PolyTerminal PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH TRUE
  )
  
  # Copy CEF binary and resource files to Qt executable's output directory
  COPY_FILES(PolyTerminal "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CMAKE_CURRENT_BINARY_DIR}")
  COPY_FILES(PolyTerminal "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}")
  
  # Set SUID permissions on chrome-sandbox
  SET_LINUX_SUID_PERMISSIONS(PolyTerminal "${CMAKE_CURRENT_BINARY_DIR}/chrome-sandbox")
endif()
