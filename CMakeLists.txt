cmake_minimum_required(VERSION 3.1)

project(jlnode
    VERSION 18.0.0
    LANGUAGES CXX)

set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_CXX_STANDARD 14)
if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
endif()

add_definitions(-DNAPI_VERSION=8)
add_definitions(-DNODE_ADDON_API_DISABLE_DEPRECATED=1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
if(NOT NAPI_INC)
    execute_process(
        COMMAND ${NODE_PATH}node -p "require('path').resolve(require('node-addon-api').include_dir)"
        OUTPUT_VARIABLE NAPI_INC
    )
endif()
string(STRIP "${NAPI_INC}" NAPI_INC)
message(STATUS "NAPI_INC: ${NAPI_INC}")

file(READ ${CMAKE_JS_INC}/node_version.h NODEJS_VERSION_FILE)

string(REGEX MATCH "#define NODE_MODULE_VERSION ([0-9]+)" NODEJS_MODULE_VERSION_DEF ${NODEJS_VERSION_FILE})
string(REGEX MATCH "([0-9]+)$" NODEJS_MODULE_VERSION ${NODEJS_MODULE_VERSION_DEF})

if(WIN32)
    set(NODEJS_LIBRARY "")
elseif(NOT NODEJS_LIBRARY)
    set(NODEJS_LIBRARY_NAMES
        libnode.so.${NODEJS_MODULE_VERSION}
        libnode.${NODEJS_MODULE_VERSION}.dylib)
    find_library(NODEJS_LIBRARY
        NAMES ${NODEJS_LIBRARY_NAMES}
        PATHS ENV LD_LIBRARY_PATH
        REQUIRED)
endif()

message(STATUS "NODEJS_LIBRARY: ${NODEJS_LIBRARY}")

set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/src/jlnode")

set(sources
    ${source_path}/module.cpp
    ${source_path}/utils.cpp
    ${source_path}/instance.cpp
    ${source_path}/externals.cpp
    ${source_path}/objects.cpp
    ${source_path}/arrays.cpp
    ${source_path}/functions.cpp
    ${source_path}/promises.cpp
    ${source_path}/threading.cpp
)
set(headers
    ${source_path}/utils.h
    ${source_path}/instance.h
    ${source_path}/externals.h
    ${source_path}/objects.h
    ${source_path}/arrays.h
    ${source_path}/functions.h
    ${source_path}/promises.h
    ${source_path}/threading.h
)

add_library(${PROJECT_NAME}
    SHARED ${sources} ${headers}
    ${CMAKE_JS_SRC})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "lib")

target_link_libraries(${PROJECT_NAME}
    ${CMAKE_JS_LIB}
    ${NODEJS_LIBRARY})

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_JS_INC}
    ${NAPI_INC})

project(jlnode_addon
    VERSION 18.0.0
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
    ${CMAKE_JS_LIB})

install(TARGETS jlnode jlnode_addon
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION lib)
