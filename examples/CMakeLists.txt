cmake_minimum_required(VERSION 3.22)
project(Examples VERSION 1.0 LANGUAGES C)

add_executable(main main.c)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_library(AFC_LIB_PATH NAMES afclib)
if(NOT AFC_LIB_PATH)
    message(FATAL_ERROR "afclib not found on the system.")
endif()

add_library(afclib STATIC IMPORTED)
set_target_properties(afclib PROPERTIES IMPORTED_LOCATION "${AFC_LIB_PATH}")
target_link_libraries(main PRIVATE afclib)