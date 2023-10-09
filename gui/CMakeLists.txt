cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall") 

project(gui)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ..)

include_directories(
    ./gui/include
)

aux_source_directory(./gui/source/ GUI_SRC_DIR)
aux_source_directory(./gui/include/ GUI_INCLUDE_DIR)

add_executable(gui
  ${GUI_INCLUDE_DIR}
  ${GUI_SRC_DIR}
)

target_link_libraries(gui
	#${Boost_SYSTEM_LIBRARY}
	${Boost_LIBRARIES}
	raylib
)
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/ressources/") # Set the asset path macro to the absolute path on the dev machine
#install(TARGETS R-Type)
#include(CPACK)
