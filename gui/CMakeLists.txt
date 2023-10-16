cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall") 
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb")

project(r-type_client)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ..)

include_directories(
    ./gui/include
)

aux_source_directory(./gui/source/ GUI_SRC_DIR)
aux_source_directory(./gui/include/ GUI_INCLUDE_DIR)

add_executable(r-type_client
  ${GUI_INCLUDE_DIR}
  ${GUI_SRC_DIR}
  ${SHARED_SRC_DIR}
 )

#add_executable(unit_tests
#  ${GUI_INCLUDE_DIR}
#  ./gui/tests/Ecs_tests.cpp
#  ./shared/Registry.cpp
#) 

target_link_libraries(r-type_client
	#${Boost_SYSTEM_LIBRARY}
	${Boost_LIBRARIES}
	raylib
)

#target_link_libraries(unit_tests
#	${Boost_LIBRARIES}
#	raylib
#  Catch2::Catch2WithMain
#)

target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/ressources/") # Set the asset path macro to the absolute path on the dev machine
#add_test(NAME unit_tests COMMAND unit_tests)
#install(TARGETS R-Type)
#include(CPACK)
