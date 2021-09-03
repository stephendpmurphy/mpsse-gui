# Set the name and the supported language of the project
project(mpsse-gui C CXX)

# Set the minimum version of cmake required to build this project
cmake_minimum_required(VERSION 3.16)

# Add our libMPSSE sub-director
add_subdirectory(vendor/libMPSSE)

# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

# Set the binary output folder
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

# Set our include directories
include_directories( src
                     vendor/libMPSSE/inc
                     vendor/libMPSSE/linux
)

set(APP_SRC ${PROJECT_SOURCE_DIR}/src/main.c
            ${PROJECT_SOURCE_DIR}/src/mpsse.c
)

# Creat our final executable
add_executable(mpsse-gui ${APP_SRC})
# Link our executable against the necessary libraries
target_link_libraries(mpsse-gui PRIVATE ${GTK_LIBRARIES} MPSSE dl ftd2xx)
# Set the necessary compilation flags
target_compile_options(mpsse-gui PRIVATE ${GTK_CFLAGS_OTHER})
# Set some custom linker flags
target_link_options(mpsse-gui PRIVATE -rdynamic)
# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
target_include_directories(mpsse-gui PRIVATE ${GTK_INCLUDE_DIRS})
target_link_directories(mpsse-gui PRIVATE ${GTK_LIBRARY_DIRS})

install(TARGETS mpsse-gui DESTINATION bin)