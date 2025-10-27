cmake_minimum_required(VERSION 3.16)

project(PolyApi VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find protobuf and gRPC using system packages
find_package(Protobuf REQUIRED)

# Find gRPC using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(grpc++ REQUIRED IMPORTED_TARGET grpc++)

# Find grpc_cpp_plugin
find_program(GRPC_CPP_PLUGIN grpc_cpp_plugin PATHS /usr/bin /usr/local/bin)
if(NOT GRPC_CPP_PLUGIN)
  message(FATAL_ERROR "grpc_cpp_plugin not found. Please install libgrpc++-dev")
endif()

# Proto compilation for polymarket
get_filename_component(pm_clob_proto "${CMAKE_CURRENT_SOURCE_DIR}/polymarket_clob.proto" ABSOLUTE)
get_filename_component(pm_gamma_proto "${CMAKE_CURRENT_SOURCE_DIR}/polymarket_gamma.proto" ABSOLUTE)
get_filename_component(pm_proto_path "${pm_clob_proto}" PATH)

set(pm_clob_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/polymarket_clob.pb.cc")
set(pm_clob_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/polymarket_clob.pb.h")
set(pm_clob_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/polymarket_clob.grpc.pb.cc")
set(pm_clob_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/polymarket_clob.grpc.pb.h")

set(pm_gamma_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/polymarket_gamma.pb.cc")
set(pm_gamma_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/polymarket_gamma.pb.h")
set(pm_gamma_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/polymarket_gamma.grpc.pb.cc")
set(pm_gamma_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/polymarket_gamma.grpc.pb.h")

add_custom_command(
  OUTPUT "${pm_clob_proto_srcs}" "${pm_clob_proto_hdrs}" "${pm_clob_grpc_srcs}" "${pm_clob_grpc_hdrs}"
  COMMAND ${Protobuf_PROTOC_EXECUTABLE}
  ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
    --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
    -I "${pm_proto_path}"
    --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN}"
    "${pm_clob_proto}"
  DEPENDS "${pm_clob_proto}")

add_custom_command(
  OUTPUT "${pm_gamma_proto_srcs}" "${pm_gamma_proto_hdrs}" "${pm_gamma_grpc_srcs}" "${pm_gamma_grpc_hdrs}"
  COMMAND ${Protobuf_PROTOC_EXECUTABLE}
  ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
    --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
    -I "${pm_proto_path}"
    --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN}"
    "${pm_gamma_proto}"
  DEPENDS "${pm_gamma_proto}")

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# Proto library
add_library(api_proto
  ${pm_clob_proto_srcs}
  ${pm_clob_proto_hdrs}
  ${pm_clob_grpc_srcs}
  ${pm_clob_grpc_hdrs}
  ${pm_gamma_proto_srcs}
  ${pm_gamma_proto_hdrs}
  ${pm_gamma_grpc_srcs}
  ${pm_gamma_grpc_hdrs})

target_link_libraries(api_proto
  PkgConfig::grpc++
  protobuf::libprotobuf)

# gRPC server executable
add_executable(BackendApi main.cpp)
target_link_libraries(BackendApi
  api_proto
  PkgConfig::grpc++
  protobuf::libprotobuf)

