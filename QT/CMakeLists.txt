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
  cef_app/CEFManager.h
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

# Disable sandbox (as per cef_app/main.cpp)
if(NOT DEFINED USE_SANDBOX)
  set(USE_SANDBOX OFF)
endif()

# CEF Standalone Executable (optional - for testing)
if(OS_LINUX)
  add_executable(cef_app_executable
    cef_app/main.cpp
  )
  
  # Determine output directory for this target
  SET_CEF_TARGET_OUT_DIR()
  set_target_properties(cef_app_executable PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CEF_TARGET_OUT_DIR}
  )
  
  # Apply CEF target properties
  SET_EXECUTABLE_TARGET_PROPERTIES(cef_app_executable)
  
  target_include_directories(cef_app_executable PRIVATE
    ${CEF_ROOT}
    ${CMAKE_CURRENT_SOURCE_DIR}/cef_app
  )
  
  target_link_libraries(cef_app_executable PRIVATE
    cef_app_lib
    libcef_dll_wrapper
    libcef_lib
    ${CEF_STANDARD_LIBS}
  )
  
  # Set rpath for CEF libraries
  set_target_properties(cef_app_executable PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH TRUE
  )
  
  # Copy CEF binary and resource files to output directory
  COPY_FILES(cef_app_executable "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
  COPY_FILES(cef_app_executable "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")
  
  # Set SUID permissions on chrome-sandbox
  SET_LINUX_SUID_PERMISSIONS(cef_app_executable "${CEF_TARGET_OUT_DIR}/chrome-sandbox")
else()
  # For non-Linux platforms
  add_executable(cef_app_executable
    cef_app/main.cpp
  )
  
  SET_EXECUTABLE_TARGET_PROPERTIES(cef_app_executable)
  
  target_include_directories(cef_app_executable PRIVATE
    ${CEF_ROOT}
    ${CMAKE_CURRENT_SOURCE_DIR}/cef_app
  )
  
  target_link_libraries(cef_app_executable PRIVATE
    cef_app_lib
    libcef_dll_wrapper
  )
endif()

# Main Qt Executable
add_executable(PolyTerminal 
  main.cpp 
  globalShortcuts.cpp 
  globalShortcuts.h
  MainWindow.cpp 
  MainWindow.h
  constants.h)

target_link_libraries(PolyTerminal PRIVATE 
  Qt6::Widgets
  cef_app_lib
)

# Include CEF headers for Qt app if needed
target_include_directories(PolyTerminal PRIVATE
  ${CEF_ROOT}
  ${CMAKE_CURRENT_SOURCE_DIR}/cef_app
)

# Qt app may also need CEF libraries if using CEF directly
if(OS_LINUX)
  # Link against libcef if Qt app directly uses CEF APIs
  # Uncomment the line below if your Qt code calls CEF functions directly
  # target_link_libraries(PolyTerminal PRIVATE libcef_lib ${CEF_STANDARD_LIBS})
  
  # Note: When using CEF from Qt, you'll need to:
  # 1. Initialize CEF (CefInitialize) from Qt's main
  # 2. Run CEF message loop in a separate thread or integrate with Qt's event loop
  # 3. Copy CEF binaries/resources to the Qt executable's output directory if needed
endif()
