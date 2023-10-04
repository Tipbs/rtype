cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb")

project(R-Type)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ..)

include_directories(
    ./include
)

aux_source_directory(source/ SRC_DIR)
aux_source_directory(include/ INCLUDE_DIR)

find_package(PkgConfig)

find_path(CRITERION_INCLUDE_DIR criterion/criterion.h
          PATH_SUFFIXES criterion)
find_library(CRITERION_LIBRARY NAMES criterion libcriterion)

set(CRITERION_LIBRARIES ${CRITERION_LIBRARY})
set(CRITERION_INCLUDE_DIRS ${CRITERION_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Criterion DEFAULT_MSG
                                  CRITERION_LIBRARY CRITERION_INCLUDE_DIR)

mark_as_advanced(CRITERION_INCLUDE_DIRS CRITERION_LIBRARIES)

# Dependencies
set(RAYLIB_VERSION 4.5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()

add_executable(unit_tests
  tests/Ecs_tests.cpp
  ${INCLUDE_DIR}
  source/Registry.cpp
)

target_link_libraries(unit_tests
    PRIVATE
    criterion
)

add_executable(R-Type
  ${INCLUDE_DIR}
  ${SRC_DIR}
)

target_link_libraries(R-Type
    PRIVATE
    raylib
)

add_test(NAME unit_tests COMMAND unit_tests)
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/ressources/") # Set the asset path macro to the absolute path on the dev machine
install(TARGETS R-Type)
