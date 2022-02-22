cmake_minimum_required(VERSION 3.21)
project(msdscript)

set(CMAKE_CXX_STANDARD 14)

include_directories(msdscript)

add_executable(msdscript
        msdscript/catch.h
        msdscript/cmdline.cpp
        msdscript/cmdline.hpp
        msdscript/expr.cpp
        msdscript/expr.hpp
        msdscript/main.cpp
        msdscript/parse.cpp
        msdscript/parse.hpp
        msdscript/Val.cpp
        msdscript/Val.hpp)
