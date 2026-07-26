cmake_minimum_required(VERSION 3.12)
project(SwitchLive VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
file(GLOB_RECURSE SWITCHLIVE_SOURCES
    wiliwili/source/*.cpp
)

file(GLOB_RECURSE SWITCHLIVE_HEADERS
    wiliwili/include/*.hpp
)

add_executable(${PROJECT_NAME}
    ${SWITCHLIVE_SOURCES}
    ${SWITCHLIVE_HEADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/wiliwili/include
)

# Dependencies
find_package(cpr REQUIRED)
find_package(nlohmann_json 3.11.0 REQUIRED)

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

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    )
else()
    # Desktop build
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

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    )
endif()

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
