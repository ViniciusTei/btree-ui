cmake_minimum_required(VERSION 3.10)

# Set the project name
project(app)

# Enable export of compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the C standard to C11 (adjust if needed)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Debug, optimization, and warning flags
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O0")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pthread")

# Set output directories for object files and archives
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set output directory for object files
set(CMAKE_OBJECT_PATH_MAX 100)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/obj)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/obj)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/obj)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/obj)

# Add raylib
add_subdirectory(raylib)

# Include raylib headers
include_directories(raylib/src)

# Find GTK
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)
include_directories(${GTK_INCLUDE_DIRS})

# Add source files
set(SOURCE_FILES src/main.c src/arvore.c src/renderer.c)

# Add executable target
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link raylib, GTK and other required libraries
target_link_libraries(${PROJECT_NAME} raylib ${GTK_LIBRARIES} GL m pthread dl)

