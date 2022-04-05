# set cmake minimum version
cmake_minimum_required(VERSION 3.16)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)

# set up name variable
set(EXE_NAME uss)

# set the project name
project (${EXE_NAME})

find_package(Boost 1.68 REQUIRED)

# add an executable
add_executable(${EXE_NAME} source/main.cpp)
target_include_directories(${EXE_NAME} PUBLIC ${Boost_INCLUDE_DIR})

