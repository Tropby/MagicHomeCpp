cmake_minimum_required(VERSION 3.0.0)
project(MagicHomeProtocol VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

if(WIN32)
    set(WINDOWS_VERSION 0x0A00) 
    add_definitions(-DWINVER=${WINDOWS_VERSION})
    add_definitions(-D_WIN32_WINNT=${WINDOWS_VERSION})
endif()

#add_definitions(-DEXTENDED_LOG)

file(GLOB SRC "src/*.cpp" "src/**/*.cpp")

include_directories(dep/ebcpp/src)

add_executable(MagicHomeProtocol ${SRC})

target_link_libraries(MagicHomeProtocol wsock32 ws2_32 -Wl,-subsystem,console)
