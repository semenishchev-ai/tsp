cmake_minimum_required(VERSION 3.14)
project(TD_TSP CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable threading support
find_package(Threads REQUIRED)

# Add source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(tsp_solver ${SOURCES})

# Link libraries
target_link_libraries(tsp_solver Threads::Threads)

# Set output directory to bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
