cmake_minimum_required(VERSION 3.12)
project(lab)

set(CMAKE_CXX_STANDARD 17)

set(TOP_DIR ${CMAKE_SOURCE_DIR})
set(SRC_DIR ${TOP_DIR}/src)

# output dirs
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${SRC_DIR})

set(SRC_LIST
        ${SRC_DIR}/BplusTree.cpp
        )

set(LIB_NAME ${PROJECT_NAME}core)

#  executable for quick testing in main.cpp
add_executable(main main.cpp src/sqlite3.h src/sqlite3.c)
target_link_libraries(main PRIVATE ${LIBRARY_NAME})



