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
file(GLOB TEST_SOURCES "tests/*.c")
enable_testing()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # 1. DEFINE OS-SPECIFIC FLAGS
    if(MSVC)
        set(DEBUG_FLAGS /fsanitize=address /Zi /Od /RTC1-)
        set(DEBUG_LINK_FLAGS /incremental:no)
    elseif(APPLE)
        # ON MAC: Disable ASan entirely to allow the 'leaks' tool to function
        set(DEBUG_FLAGS -g -O0)
        set(DEBUG_LINK_FLAGS "")
    else()
        # ON LINUX: Keep ASan and LeakSanitizer active
        set(DEBUG_FLAGS -fsanitize=address -fsanitize=leak -fno-omit-frame-pointer -g)
        set(DEBUG_LINK_FLAGS -fsanitize=address -fsanitize=leak)
    endif()

    # 2. APPLY TO LIBRARY
    target_compile_options(${PROJECT_NAME} PRIVATE ${DEBUG_FLAGS})
    target_link_options(${PROJECT_NAME} PRIVATE ${DEBUG_LINK_FLAGS})

    # 3. APPLY TO TESTS
    foreach(test_file ${TEST_SOURCES})
        get_filename_component(test_name ${test_file} NAME_WE)
        
        if(NOT TARGET ${test_name})
            add_executable(${test_name} ${test_file})
            target_link_libraries(${test_name} PRIVATE ${PROJECT_NAME})
        endif()

        target_compile_options(${test_name} PRIVATE ${DEBUG_FLAGS})
        target_link_options(${test_name} PRIVATE ${DEBUG_LINK_FLAGS})

        if(APPLE)
            # Use native macOS tool for the actual test command
            add_test(NAME ${test_name} 
                 COMMAND bash -c "$<TARGET_FILE:${test_name}> && leaks -quiet -atExit -- $<TARGET_FILE:${test_name}> | grep -v 'Binary Images:'"
            )
        else()
            add_test(NAME ${test_name} COMMAND ${test_name})
        endif()
    endforeach()
endif()
