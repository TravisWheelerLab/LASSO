# set cmake minimum version
cmake_minimum_required(VERSION 3.16)

# set up name variable
set(name "unbounded_subset_sum")

# set the project name
project (${name})

# add an executable
add_executable(${name}.exe main.cpp)
