# Copyright (C) 2022  David Capello

cmake_minimum_required(VERSION 3.2)

project(utf8lab CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX AND NOT APPLE)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
endif()

include(ExternalProject)
ExternalProject_Add(googlebenchmark-project
  URL https://github.com/google/benchmark/archive/master.zip
  PREFIX "${CMAKE_BINARY_DIR}/googlebenchmark"
  INSTALL_DIR "${CMAKE_BINARY_DIR}/googlebenchmark"
  BUILD_BYPRODUCTS "${CMAKE_BINARY_DIR}/googlebenchmark/lib/${CMAKE_STATIC_LIBRARY_PREFIX}benchmark${CMAKE_STATIC_LIBRARY_SUFFIX}"
  CMAKE_CACHE_ARGS
    -DBENCHMARK_ENABLE_GTEST_TESTS:BOOL=OFF
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DCMAKE_INSTALL_LIBDIR:PATH=<INSTALL_DIR>/lib)

ExternalProject_Get_Property(googlebenchmark-project install_dir)
set(GOOGLEBENCHMARK_INCLUDE_DIRS ${install_dir}/include)
set(GOOGLEBENCHMARK_LIBRARY ${install_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}benchmark${CMAKE_STATIC_LIBRARY_SUFFIX})

# Create the directory so changing INTERFACE_INCLUDE_DIRECTORIES doesn't fail
file(MAKE_DIRECTORY ${GOOGLEBENCHMARK_INCLUDE_DIRS})

add_library(googlebenchmark STATIC IMPORTED)
set_target_properties(googlebenchmark PROPERTIES
  IMPORTED_LOCATION ${GOOGLEBENCHMARK_LIBRARY}
  INTERFACE_INCLUDE_DIRECTORIES ${GOOGLEBENCHMARK_INCLUDE_DIRS})
add_dependencies(googlebenchmark googlebenchmark-project)

add_executable(benchmarks benchmarks.cpp)
target_link_libraries(benchmarks googlebenchmark)
if(WIN32)
  target_link_libraries(benchmarks shlwapi)
endif()

# Test data
file(DOWNLOAD
  https://www.w3.org/2001/06/utf-8-test/UTF-8-demo.html
  ${CMAKE_CURRENT_BINARY_DIR}/utf8.data)
