cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall") 

project(server)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ..)

include_directories(
    ./include
)

aux_source_directory(./server/source/ SRC_DIR)
aux_source_directory(./server/include/ INCLUDE_DIR)

add_executable(server
  ${INCLUDE_DIR}
  ${SRC_DIR}
)

target_link_libraries(server
	#${Boost_FILESYSTEM_LIBRARY}
	#${Boost_SYSTEM_LIBRARY}
	${Boost_LIBRARIES}
)
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/ressources/") # Set the asset path macro to the absolute path on the dev machine
#install(TARGETS R-Type)
#include(CPACK)