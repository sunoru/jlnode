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

include_directories(${CMAKE_JS_INC})

add_library(${PROJECT_NAME}
    SHARED ${sources} ${headers}
    ${CMAKE_JS_SRC})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

target_link_libraries(${PROJECT_NAME}
        JlCxx::cxxwrap_julia
        ${CMAKE_JS_LIB})

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION lib)
