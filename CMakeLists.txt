cmake_minimum_required(VERSION 3.1.2)
project(bind)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(SOURCE_FILES
    bind-testing.cpp
    bind.h
#    main.cpp
        )

add_executable(bind ${SOURCE_FILES})