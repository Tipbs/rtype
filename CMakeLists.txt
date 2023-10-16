if(MSVC)
  add_definitions("/EHsc")
endif(MSVC)

cmake_minimum_required(VERSION 3.20.3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall") 

aux_source_directory(./shared SHARED_SRC_DIR)

project(R-Type)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ..)

find_package(Boost 1.82.0 COMPONENTS system filesystem regex asio serialization REQUIRED)
find_package(raylib REQUIRED)
find_package(Catch2 REQUIRED)

include(./server/CMakeLists.txt)
include(./gui/CMakeLists.txt)
include(CPack)
