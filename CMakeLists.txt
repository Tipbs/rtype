if(MSVC)
  add_definitions("/EHsc")
endif(MSVC)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall") 

set(CPACK_PACKAGE_NAME "RType")

project(R-Type)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ..)

find_package(Boost 1.82.0 COMPONENTS system filesystem regex asio REQUIRED)

include(CPACK)
include(./server/CMakeLists.txt)
