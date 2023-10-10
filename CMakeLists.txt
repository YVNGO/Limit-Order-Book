cmake_minimum_required(VERSION 3.11-3.18)

project(
    LimitOrderBook
    VERSION 0.1
    DESCRIPTION "Simple implementation of a limit order book")

set(SOURCE_FILES main.cpp)
add_executable(LimitOrderBook ${SOURCE_FILES})

include_directories(src)

add_subdirectory(src)   