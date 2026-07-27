cmake_minimum_required(VERSION 3.14)

# borealis integration - must come before project()
set(BOREALIS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/library/borealis)
set(BOREALIS_LIBRARY ${BOREALIS_DIR}/library)

include(${BOREALIS_LIBRARY}/cmake/commonOption.cmake)
include(${BOREALIS_LIBRARY}/cmake/toolchain.cmake)

project(SwitchLive VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies
include(FetchContent)

FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG        1.9.9
    GIT_SHALLOW    TRUE
)
set(CPR_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
set(CPR_ENABLE_SSL ON CACHE BOOL "" FORCE)

if (PLATFORM_SWITCH)
    set(CPR_FORCE_USE_SYSTEM_CURL ON CACHE BOOL "" FORCE)
    set(CPR_FORCE_MBEDTLS_BACKEND ON CACHE BOOL "" FORCE)
else ()
    set(CPR_FORCE_USE_SYSTEM_CURL OFF CACHE BOOL "" FORCE)
endif ()

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
    GIT_SHALLOW    TRUE
)
set(JSON_BuildTests OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(cpr json)

# Build borealis library
add_subdirectory(library/borealis/library)

# nanovg font hash size
add_definitions(-DFONS_HASH_LUT_SIZE=4096)

# Project sources
file(GLOB_RECURSE SWITCHLIVE_SOURCES wiliwili/source/*.cpp)

list(APPEND SWITCHLIVE_SOURCES ${BOREALIS_LIBRARY}/lib/platforms/switch/switch_wrapper.c)

add_executable(${PROJECT_NAME} ${SWITCHLIVE_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/wiliwili/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    borealis
    cpr::cpr
    nlohmann_json::nlohmann_json
)

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_THREAD_LIBS_INIT})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -ffunction-sections -fdata-sections)

# NRO build for Switch
if (PLATFORM_SWITCH)
    add_custom_target(${PROJECT_NAME}.nro
        DEPENDS ${PROJECT_NAME}
        COMMAND ${NX_NACPTOOL_EXE} --create ${PROJECT_NAME} "SwitchLive" "0.1.0" ${PROJECT_NAME}.nacp
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/romfs
        COMMAND ${NX_ELF2NRO_EXE} ${PROJECT_NAME}.elf ${PROJECT_NAME}.nro
            --nacp=${PROJECT_NAME}.nacp
            --romfsdir=${CMAKE_BINARY_DIR}/romfs
        ALL
    )
endif()
