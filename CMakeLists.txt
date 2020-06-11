cmake_minimum_required(VERSION 3.10)
project(draup)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wunused")

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
find_package(Boost REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

add_executable(draup main.cpp)
