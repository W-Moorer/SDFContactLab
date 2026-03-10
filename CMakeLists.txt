cmake_minimum_required(VERSION 3.24)

project(SDFContactLab VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SDF_CONTACT_LAB_FETCH_DEPS "Fetch third-party dependencies automatically" ON)
option(SDF_CONTACT_LAB_BUILD_TESTS "Build tests" ON)

include(FetchContent)

function(sdf_contact_lab_require_eigen)
    find_package(Eigen3 3.4 CONFIG QUIET)
    if(Eigen3_FOUND)
        return()
    endif()

    if(NOT SDF_CONTACT_LAB_FETCH_DEPS)
        message(FATAL_ERROR "Eigen3 was not found and SDF_CONTACT_LAB_FETCH_DEPS=OFF.")
    endif()

    FetchContent_Declare(
        eigen
        URL https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
    )
    FetchContent_GetProperties(eigen)
    if(NOT eigen_POPULATED)
        FetchContent_Populate(eigen)
    endif()

    add_library(Eigen3::Eigen INTERFACE IMPORTED)
    set_target_properties(Eigen3::Eigen PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${eigen_SOURCE_DIR}"
    )
endfunction()

function(sdf_contact_lab_require_json)
    find_package(nlohmann_json 3.11 CONFIG QUIET)
    if(nlohmann_json_FOUND)
        return()
    endif()

    if(NOT SDF_CONTACT_LAB_FETCH_DEPS)
        message(FATAL_ERROR "nlohmann_json was not found and SDF_CONTACT_LAB_FETCH_DEPS=OFF.")
    endif()

    FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    )
    FetchContent_MakeAvailable(nlohmann_json)
endfunction()

function(sdf_contact_lab_require_catch2)
    if(NOT SDF_CONTACT_LAB_BUILD_TESTS)
        return()
    endif()

    find_package(Catch2 3 CONFIG QUIET)
    if(Catch2_FOUND)
        return()
    endif()

    if(NOT SDF_CONTACT_LAB_FETCH_DEPS)
        message(FATAL_ERROR "Catch2 was not found and SDF_CONTACT_LAB_FETCH_DEPS=OFF.")
    endif()

    FetchContent_Declare(
        Catch2
        URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.6.0.zip
    )
    FetchContent_MakeAvailable(Catch2)
endfunction()

sdf_contact_lab_require_eigen()
sdf_contact_lab_require_json()
sdf_contact_lab_require_catch2()

add_library(sdf_mbd
    src/core/config.cpp
    src/core/logger.cpp
    src/geometry/analytic_sdfs.cpp
    src/contact/simple_contact_pipeline.cpp
    src/dynamics/rigid_body.cpp
    src/eval/metric_calculator.cpp
    src/eval/result_writer.cpp
)

target_include_directories(sdf_mbd
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(sdf_mbd
    PUBLIC
        Eigen3::Eigen
        nlohmann_json::nlohmann_json
)

target_compile_definitions(sdf_mbd
    PUBLIC
        SDF_CONTACT_LAB_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

if(MSVC)
    target_compile_options(sdf_mbd PRIVATE /W4 /permissive- /FS)
else()
    target_compile_options(sdf_mbd PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(demo_hello src/apps/demo_hello.cpp)
target_link_libraries(demo_hello PRIVATE sdf_mbd)
if(MSVC)
    target_compile_options(demo_hello PRIVATE /FS)
endif()

add_executable(benchmark_runner src/apps/benchmark_runner.cpp)
target_link_libraries(benchmark_runner PRIVATE sdf_mbd)
if(MSVC)
    target_compile_options(benchmark_runner PRIVATE /FS)
endif()

if(SDF_CONTACT_LAB_BUILD_TESTS)
    enable_testing()

    add_executable(sdf_contact_lab_tests
        tests/test_sdf.cpp
        tests/test_config_and_pipeline.cpp
    )

    target_link_libraries(sdf_contact_lab_tests
        PRIVATE
            sdf_mbd
            Catch2::Catch2WithMain
    )
    if(MSVC)
        target_compile_options(sdf_contact_lab_tests PRIVATE /FS)
    endif()

    add_test(NAME sdf_contact_lab_tests COMMAND sdf_contact_lab_tests)
endif()
