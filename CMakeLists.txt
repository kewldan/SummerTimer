cmake_minimum_required(VERSION 3.26)
project(SummerTimer)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        include
)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

file(GLOB_RECURSE SRCS src/*.cpp)

add_executable(SummerTimer ${SRCS})
set_property(TARGET SummerTimer PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_link_libraries(SummerTimer PRIVATE sfml-system sfml-graphics sfml-window)
