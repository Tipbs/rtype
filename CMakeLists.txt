cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall") 

set(CPACK_PACKAGE_NAME "RType")

project(R-Type)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ..)

include_directories(
    ./include
)

aux_source_directory(source/ SRC_DIR)
aux_source_directory(include/ INCLUDE_DIR)

#find_package(raylib 4.0.0 REQUIRED)

add_executable(R-Type
  ${INCLUDE_DIR}
  ${SRC_DIR}
)

target_link_libraries(R-Type
#raylib
)
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/ressources/") # Set the asset path macro to the absolute path on the dev machine
install(TARGETS R-Type)
include(CPACK)
