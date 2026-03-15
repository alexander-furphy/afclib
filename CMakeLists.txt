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

# ---------------- Debug and Sanitizer Setup ----------------
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MSVC)
        set(DEBUG_FLAGS /fsanitize=address /Zi /Od /RTC1-)
        set(DEBUG_LINK_FLAGS /incremental:no)
    else()
        # Basic ASan for both Mac and Linux
        set(DEBUG_FLAGS -fsanitize=address -fno-omit-frame-pointer -g)
        set(DEBUG_LINK_FLAGS -fsanitize=address)

        if(NOT APPLE)
            # Linux gets the explicit flags
            list(APPEND DEBUG_FLAGS -fsanitize=leak)
            list(APPEND DEBUG_LINK_FLAGS -fsanitize=leak)
        endif()
    endif()

    # Apply to Library
    target_compile_options(${PROJECT_NAME} PRIVATE ${DEBUG_FLAGS})
    target_link_options(${PROJECT_NAME} PRIVATE ${DEBUG_LINK_FLAGS})

    # Apply to Tests and fix the "abort" on Mac
    foreach(test_file ${TEST_SOURCES})
        get_filename_component(test_name ${test_file} NAME_WE)
        
        # Ensure target is defined before setting properties
        if(NOT TARGET ${test_name})
            add_executable(${test_name} ${test_file})
            target_link_libraries(${test_name} PRIVATE ${PROJECT_NAME})
        endif()

        target_compile_options(${test_name} PRIVATE ${DEBUG_FLAGS})
        target_link_options(${test_name} PRIVATE ${DEBUG_LINK_FLAGS})

        if(APPLE)
            # FORCE Mac to ignore leaks during the ASan run to prevent the abort
            # Then wrap it in the 'leaks' tool to actually find the leak
            set_tests_properties(${test_name} PROPERTIES ENVIRONMENT "ASAN_OPTIONS=detect_leaks=0")
            add_test(NAME ${test_name} COMMAND leaks -atExit -- $<TARGET_FILE:${test_name}>)
        else()
            # Linux/Windows run normally
            add_test(NAME ${test_name} COMMAND ${test_name})
        endif()
    endforeach()
endif()
