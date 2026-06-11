cmake_minimum_required(VERSION 3.10)
project(FoodOrderSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "*.cpp" "sqlite3.c")

add_executable(FoodOrderSystem ${SOURCES})

target_include_directories(FoodOrderSystem PRIVATE .)