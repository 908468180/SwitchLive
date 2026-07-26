cmake_minimum_required(VERSION 3.14)
project(SwitchLive VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch dependencies
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG        v1.11.0
    GIT_SHALLOW    TRUE
)
set(CPR_USE_SYSTEM_CURL OFF CACHE BOOL "" FORCE)
set(CPR_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
set(CPR_ENABLE_CPP OPTIONS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
    GIT_SHALLOW    TRUE
)
set(JSON_BuildTests OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(cpr json)

# Source files
file(GLOB_RECURSE SWITCHLIVE_SOURCES wiliwili/source/*.cpp)
file(GLOB_RECURSE SWITCHLIVE_HEADERS wiliwili/include/*.hpp)

add_executable(${PROJECT_NAME}
    ${SWITCHLIVE_SOURCES}
    ${SWITCHLIVE_HEADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/wiliwili/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    cpr::cpr
    nlohmann_json::nlohmann_json
)

# Nintendo Switch
if(DEFINED PLATFORM_SWITCH)
    message(STATUS "Building for Nintendo Switch")
    set(DEVKITPRO $ENV{DEVKITPRO})
    include(${DEVKITPRO}/switch.cmake)

    target_link_libraries(${PROJECT_NAME} PRIVATE
        switch::libswitch
        switch::mm
        switch::websockets
    )

    target_compile_definitions(${PROJECT_NAME} PRIVATE
        __SWITCH__
        __NX__
        PLATFORM_SWITCH
        BOREALIS_USE_OPENGL
    )
else()
    # Desktop
    find_package(Threads REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)

    target_link_libraries(${PROJECT_NAME} PRIVATE
        Threads::Threads
        OpenGL::GL
        glfw
    )

    target_compile_definitions(${PROJECT_NAME} PRIVATE
        PLATFORM_DESKTOP
        BOREALIS_USE_OPENGL
    )
endif()

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
