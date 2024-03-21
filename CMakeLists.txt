cmake_minimum_required(VERSION 3.28)
project(scel_format)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(scel_format src/main.cpp
        src/value.hpp
        src/scel_transformer.cpp
        src/scel_transformer.hpp)
target_compile_features(scel_format PRIVATE cxx_std_20)
