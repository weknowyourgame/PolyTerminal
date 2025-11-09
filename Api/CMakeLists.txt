cmake_minimum_required(VERSION 3.18)

project(PolyApi VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
        httplib
        GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
        GIT_TAG        v0.15.3
)
FetchContent_MakeAvailable(httplib)

# Find required packages
find_package(Protobuf REQUIRED)
find_package(OpenSSL REQUIRED)

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
get_filename_component(pm_proto_path "${pm_clob_proto}" PATH)

set(pm_clob_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/polymarket_clob.pb.cc")
set(pm_clob_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/polymarket_clob.pb.h")
set(pm_clob_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/polymarket_clob.grpc.pb.cc")
set(pm_clob_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/polymarket_clob.grpc.pb.h")

add_custom_command(
        OUTPUT "${pm_clob_proto_srcs}" "${pm_clob_proto_hdrs}" "${pm_clob_grpc_srcs}" "${pm_clob_grpc_hdrs}"
        COMMAND ${Protobuf_PROTOC_EXECUTABLE}
        ARGS --experimental_allow_proto3_optional
        --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${pm_proto_path}"
        --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN}"
        "${pm_clob_proto}"
        DEPENDS "${pm_clob_proto}")

# Proto compilation for database layer
get_filename_component(db_layer_proto "${CMAKE_CURRENT_SOURCE_DIR}/database_layer.proto" ABSOLUTE)

set(db_layer_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/database_layer.pb.cc")
set(db_layer_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/database_layer.pb.h")
set(db_layer_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/database_layer.grpc.pb.cc")
set(db_layer_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/database_layer.grpc.pb.h")

add_custom_command(
        OUTPUT "${db_layer_proto_srcs}" "${db_layer_proto_hdrs}" "${db_layer_grpc_srcs}" "${db_layer_grpc_hdrs}"
        COMMAND ${Protobuf_PROTOC_EXECUTABLE}
        ARGS --experimental_allow_proto3_optional
        --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${pm_proto_path}"
        --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN}"
        "${db_layer_proto}"
        DEPENDS "${db_layer_proto}")

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# Proto library
add_library(api_proto
        ${pm_clob_proto_srcs}
        ${pm_clob_proto_hdrs}
        ${pm_clob_grpc_srcs}
        ${pm_clob_grpc_hdrs}
        ${db_layer_proto_srcs}
        ${db_layer_proto_hdrs}
        ${db_layer_grpc_srcs}
        ${db_layer_grpc_hdrs})

target_link_libraries(api_proto
        PkgConfig::grpc++
        protobuf::libprotobuf
        httplib::httplib
)

# gRPC server executable
add_executable(BackendApi main.cpp database.cpp)
target_link_libraries(BackendApi
        api_proto
        PkgConfig::grpc++
        protobuf::libprotobuf
        httplib::httplib
        OpenSSL::SSL
        OpenSSL::Crypto
        mysqlcppconn
)
