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
        ${source_path}/instance.cpp)
set(headers
        ${source_path}/instance.h)

include_directories(${CMAKE_JS_INC})

add_library(${PROJECT_NAME}
    SHARED ${sources} ${headers}
    ${CMAKE_JS_SRC})

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
        PATHS ${CMAKE_CURRENT_SOURCE_DIR}/node/out/${CMAKE_BUILD_TYPE})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

target_link_libraries(${PROJECT_NAME}
        JlCxx::cxxwrap_julia
        ${CMAKE_JS_LIB}
        ${NODEJS_LIBRARY})

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/node/src
        ${CMAKE_CURRENT_SOURCE_DIR}/node/deps/v8/include
        ${CMAKE_CURRENT_SOURCE_DIR}/node/deps/uv/include)

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION lib)
