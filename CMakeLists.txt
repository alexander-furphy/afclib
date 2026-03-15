# ---------------- Project configuration ----------------
cmake_minimum_required(VERSION 3.15)
project(afclib VERSION 0.1.0 LANGUAGES C)

# ---------------- C configuration ----------------
set(C_STANDARD_REQUIRED 99)
set(C_STANDARD_REQUIRED ON)

# ---------------- Collect all sources and headers ----------------
file(GLOB SOURCES "src/*.c")
file(GLOB HEADERS "include/*.h")

# ---------------- Library setup ----------------
add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# ---------------- Archive / Installation ----------------
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# ---------------- Create a dist Archive ----------------
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_GENERATOR "TGZ;ZIP")
include(CPack)

# ---------------- Testing setup ----------------
enable_testing()

file(GLOB TEST_SOURCES "tests/*.c")

foreach(test_file ${TEST_SOURCES})
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file})
    target_link_libraries(${test_name} PRIVATE ${PROJECT_NAME})
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()

# ---------------- Debug and Sanitizer Setup ----------------
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /fsanitize=address)
    else()
        # Common flags for ASan and LeakSanitizer
        set(DEBUG_FLAGS -fsanitize=address -fsanitize=address -fno-omit-frame-pointer -g)

        if(NOT APPLE)
            list(APPEND DEBUG_FLAGS -fsanitize=leak)
        endif()

        target_compile_options(${PROJECT_NAME} PRIVATE ${DEBUG_FLAGS})
        target_link_options(${PROJECT_NAME} PRIVATE ${DEBUG_FLAGS})

        # Apply to tests as well so they catch leaks in the library
        foreach(test_file ${TEST_SOURCES})
            get_filename_component(test_name ${test_file} NAME_WE)
            target_compile_options(${test_name} PRIVATE ${DEBUG_FLAGS})
            target_link_options(${test_name} PRIVATE ${DEBUG_FLAGS})
        endforeach()
    endif()
endif()