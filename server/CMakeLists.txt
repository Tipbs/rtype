cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall") 
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb")

project(r-type_server)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ..)

include_directories(
    ./server/include
)

aux_source_directory(./server/source/ SERVER_SRC_DIR)
aux_source_directory(./server/include/ SERVER_INCLUDE_DIR)

add_executable(r-type_server
  ${SERVER_INCLUDE_DIR}
  ${SERVER_SRC_DIR}
  ${SHARED_SRC_DIR}
)

target_compile_definitions(r-type_server PRIVATE SERVER=1)

target_link_libraries(r-type_server
	#${Boost_FILESYSTEM_LIBRARY}
	#${Boost_SYSTEM_LIBRARY}
	${Boost_LIBRARIES}
)
#target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/ressources/") # Set the asset path macro to the absolute path on the dev machine
#install(TARGETS R-Type)
#include(CPACK)
