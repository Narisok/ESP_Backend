cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

add_compile_definitions(NII_EXCEPTIONS_ENABLED)
add_compile_definitions(DEBUG)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


project(Main)

add_subdirectory("${PROJECT_SOURCE_DIR}/src")
# add_subdirectory("${PROJECT_SOURCE_DIR}/includes")

# add_definitions(-include config)

add_executable(Main ${SOURCES} main.cpp)


target_include_directories(Main PUBLIC ${HEADERS})
