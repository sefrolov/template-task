cmake_minimum_required(VERSION 3.24...3.31)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ConfigureCompiler)
include(CompilerWarnings)

project(template LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

ct_configure_compiler()
include(Dependencies)

find_package(Python3 COMPONENTS Interpreter REQUIRED)
find_program(IWYU_BIN NAMES include-what-you-use iwyu)
execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpfullversion
        OUTPUT_VARIABLE _gcc_ver_full
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET)
execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpmachine
        OUTPUT_VARIABLE _triple
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET)
string(REGEX REPLACE "^([0-9]+)\\..*" "\\1" _gcc_ver "${_gcc_ver_full}")
set(_root1 "/usr/include/c++/${_gcc_ver}")
set(_root2 "/usr/include/${_triple}/c++/${_gcc_ver}")
execute_process(
        COMMAND ${Python3_EXECUTABLE} "${CMAKE_SOURCE_DIR}/cmake/iwyu-mapgen-libstdcxx.py"
        --lang imp "${_root1}" "${_root2}"
        OUTPUT_FILE "${CMAKE_SOURCE_DIR}/cmake/libstdcxx.imp"
        ERROR_VARIABLE _mapgen_err
        RESULT_VARIABLE _mapgen_rc
)

if (IWYU_BIN)
    if (MSVC)
        set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "${IWYU_BIN};--driver-mode=cl"
                "-Wno-unknown-warning-option"
                "-Xiwyu;--mapping_file=${CMAKE_SOURCE_DIR}/cmake/libstdcxx.imp")
    else ()
        set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "${IWYU_BIN}"
                "-Wno-unknown-warning-option"
                "-Xiwyu;--mapping_file=${CMAKE_SOURCE_DIR}/cmake/libstdcxx.imp")
    endif ()
else ()
    message(WARNING "include-what-you-use not found, running without it")
endif ()

# Setup a 'solution' target
file(GLOB SOLUTION_SOURCES CONFIGURE_DEPENDS src/*.cpp)
list(LENGTH SOLUTION_SOURCES SOLUTION_SOURCES_LENGTH)
if (SOLUTION_SOURCES_LENGTH EQUAL 0)
    add_library(solution INTERFACE)
    target_include_directories(solution INTERFACE src)
else ()
    add_library(solution ${SOLUTION_SOURCES}
            src/solution.cpp
            src/solution2.cpp
            src/correct_solution.cpp
    )
    target_include_directories(solution PUBLIC src)
    set_target_properties(solution PROPERTIES LINKER_LANGUAGE CXX)
    ct_set_compiler_warnings(solution)
endif ()

# Setup a 'tests' target
file(GLOB TESTS_SOURCES CONFIGURE_DEPENDS test/*.cpp)
add_executable(tests ${TESTS_SOURCES})
target_include_directories(tests PRIVATE test)
ct_set_compiler_warnings(tests)

# Link tests with solution
target_link_libraries(tests PRIVATE solution)

# Link tests with dependencies
find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
