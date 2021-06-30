cmake_minimum_required(VERSION 3.1)

project(jlnode
        VERSION 14.17.1
        LANGUAGES CXX)

set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

find_package(JlCxx)
get_target_property(JlCxx_location JlCxx::cxxwrap_julia LOCATION)
get_filename_component(JlCxx_location ${JlCxx_location} DIRECTORY)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib;${JlCxx_location}")

message(STATUS "Found JlCxx at ${JlCxx_location}")

set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(sources
        ${source_path}/module.cpp
        ${source_path}/NodeJSInstance.cpp)
set(headers
        ${source_path}/NodeJSInstance.h)
add_library(jlnode SHARED ${sources} ${headers})

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/node/src/node_version.h NODEJS_VERSION_FILE)

string(REGEX MATCH "#define NODE_MODULE_VERSION ([0-9]+)" NODEJS_MODULE_VERSION_DEF ${NODEJS_VERSION_FILE})
string(REGEX MATCH "([0-9]+)$" NODEJS_MODULE_VERSION ${NODEJS_MODULE_VERSION_DEF})

set(NODEJS_LIBRARY_NAMES
        libnode.so.${NODEJS_MODULE_VERSION}
        libnode.${NODEJS_MODULE_VERSION}.dylib
        libnode.${NODEJS_MODULE_VERSION}.dll
        node.lib)

find_library(NODEJS_LIBRARY
        NAMES ${NODEJS_LIBRARY_NAMES}
        PATHS ${CMAKE_CURRENT_SOURCE_DIR}/node/out/Release)

target_link_libraries(jlnode
        JlCxx::cxxwrap_julia
        ${NODEJS_LIBRARY})

target_include_directories(jlnode
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/node/src
        ${CMAKE_CURRENT_SOURCE_DIR}/node/deps/v8/include
        ${CMAKE_CURRENT_SOURCE_DIR}/node/deps/uv/include)

add_executable(test_run
        ${sources}
        ${headers}
        test/test_run.cpp)

target_link_libraries(test_run
        JlCxx::cxxwrap_julia
        ${NODEJS_LIBRARY})

target_include_directories(test_run
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/node/src
        ${CMAKE_CURRENT_SOURCE_DIR}/node/deps/v8/include
        ${CMAKE_CURRENT_SOURCE_DIR}/node/deps/uv/include)

install(TARGETS jlnode test_run
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION lib)