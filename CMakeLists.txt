cmake_minimum_required(VERSION 3.1)

project(jlnode
    VERSION 14.17.1
    LANGUAGES CXX)

set(CMAKE_MACOSX_RPATH 1)
if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
endif()

add_definitions(-DNAPI_VERSION=8)

if(NOT NAPI_INC)
    execute_process(
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/node/out/${CMAKE_BUILD_TYPE}/node -p "require('path').resolve(require('node-addon-api').include_dir)"
        OUTPUT_VARIABLE NAPI_INC
    )
    if (NOT NAPI_INC)
        execute_process(
            COMMAND node -p "require('path').resolve(require('node-addon-api').include_dir)"
            OUTPUT_VARIABLE NAPI_INC
        )
    endif()
endif()
message(STATUS "NAPI_INC: ${NAPI_INC}")

file(READ ${CMAKE_JS_INC}/node_version.h NODEJS_VERSION_FILE)

string(REGEX MATCH "#define NODE_MODULE_VERSION ([0-9]+)" NODEJS_MODULE_VERSION_DEF ${NODEJS_VERSION_FILE})
string(REGEX MATCH "([0-9]+)$" NODEJS_MODULE_VERSION ${NODEJS_MODULE_VERSION_DEF})

set(NODEJS_LIBRARY_NAMES
    libnode.so.${NODEJS_MODULE_VERSION}
    libnode.${NODEJS_MODULE_VERSION}.dylib
    libnode.${NODEJS_MODULE_VERSION}.dll
    node.lib)

find_library(NODEJS_LIBRARY
    NAMES ${NODEJS_LIBRARY_NAMES}
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/node/out/${CMAKE_BUILD_TYPE}
    REQUIRED)

set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/src/jlnode")

set(sources
    ${source_path}/module.cpp
    ${source_path}/instance.cpp)
set(headers
    ${source_path}/instance.h)

add_library(${PROJECT_NAME}
    SHARED ${sources} ${headers}
    ${CMAKE_JS_SRC})

target_link_libraries(${PROJECT_NAME}
    ${CMAKE_JS_LIB}
    ${NODEJS_LIBRARY})

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/node/src
    ${CMAKE_CURRENT_SOURCE_DIR}/node/deps/v8/include
    ${CMAKE_CURRENT_SOURCE_DIR}/node/deps/uv/include
    ${CMAKE_JS_INC}
    ${NAPI_INC})

project(jlnode_addon
    VERSION 14.17.1
    LANGUAGES CXX)

set(addon_path "${CMAKE_CURRENT_SOURCE_DIR}/src/addon")
set(addon_sources
    ${addon_path}/addon.cpp
    ${addon_path}/init.cpp)
set(addon_headers
    ${addon_path}/init.h)

add_library(${PROJECT_NAME}
    SHARED ${addon_sources} ${addon_headers}
    ${CMAKE_JS_SRC})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_JS_INC}
    ${NAPI_INC})

target_link_libraries(${PROJECT_NAME}
    ${CMAKE_JS_LIB}
    ${NODEJS_LIBRARY})

install(TARGETS jlnode jlnode_addon
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION lib)

