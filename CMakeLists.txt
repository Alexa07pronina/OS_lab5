cmake_minimum_required(VERSION 3.16)
project(OS_Lab5_Variant34 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<0:>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<0:>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<0:>")
if(APPLE)
    set(CMAKE_BUILD_RPATH "@loader_path")
elseif(UNIX)
    set(CMAKE_BUILD_RPATH "$ORIGIN")
    set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
endif()

add_library(impl1 SHARED src/impl1.cpp)
add_library(impl2 SHARED src/impl2.cpp)
foreach(library impl1 impl2)
    target_include_directories(${library} PUBLIC include)
    target_compile_definitions(${library} PRIVATE LAB5_EXPORTS)
endforeach()

set(LINKED_IMPL "1" CACHE STRING "Implementation for program1: 1 or 2")
set_property(CACHE LINKED_IMPL PROPERTY STRINGS 1 2)
if(NOT LINKED_IMPL MATCHES "^[12]$")
    message(FATAL_ERROR "LINKED_IMPL must be 1 or 2")
endif()
add_executable(program1 src/program1.cpp)
target_compile_definitions(program1 PRIVATE LAB5_IMPORTS)
target_link_libraries(program1 PRIVATE impl${LINKED_IMPL})

add_executable(program2 src/program2.cpp)
target_include_directories(program2 PRIVATE include)
target_compile_definitions(program2 PRIVATE
    LAB5_LIBRARY1="$<TARGET_FILE_NAME:impl1>"
    LAB5_LIBRARY2="$<TARGET_FILE_NAME:impl2>")
if(CMAKE_DL_LIBS)
    target_link_libraries(program2 PRIVATE ${CMAKE_DL_LIBS})
endif()
add_dependencies(program2 impl1 impl2)

if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(program2 PRIVATE -Wno-cast-function-type)
endif()

foreach(target impl1 impl2 program1 program2)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra)
    endif()
endforeach()

include(CTest)
if(BUILD_TESTING)
    include(CheckTypeSize)
    check_type_size("long" LAB5_LONG_SIZE LANGUAGE CXX)
    add_test(NAME lab5_integration COMMAND ${CMAKE_COMMAND}
        "-DPROGRAM1=$<TARGET_FILE:program1>"
        "-DPROGRAM2=$<TARGET_FILE:program2>"
        "-DIMPL1=$<TARGET_FILE:impl1>"
        "-DIMPL2=$<TARGET_FILE:impl2>"
        "-DLINKED_IMPL=${LINKED_IMPL}"
        "-DLONG_SIZE=${LAB5_LONG_SIZE}"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/tests/check.cmake")
endif()
