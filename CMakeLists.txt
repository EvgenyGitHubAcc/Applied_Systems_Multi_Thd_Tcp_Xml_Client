cmake_minimum_required(VERSION 3.5)

project(AppSysImplClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Libs/pugixml-1.10)
include_directories("Libs/pugixml-1.10/src")

add_executable(AppSysImplClient main.cpp network.cpp xml.cpp)

target_link_libraries(AppSysImplClient pugixml ws2_32)
