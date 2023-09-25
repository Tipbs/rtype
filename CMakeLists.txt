
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall") 

project(R-Type)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ..)

include_directories(
    ./include
)

aux_source_directory(source/ SRC_DIR)
aux_source_directory(include/ INCLUDE_DIR)

# Dependencies
set(LIBXRANDR_VERSION 1.5.0)
find_package(libxrandr ${LIBXRANDR_VERSION} QUIET) # QUIET or REQUIRED
if (NOT libxrandr_FOUND) # If there's none, fetch and build libxrandr
  include(FetchContent)
  FetchContent_Declare(
    libxrandr
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/xtforever/libXrandr/archive/refs/tags/libXrandr-1.5.0.tar.gz
  )
  FetchContent_GetProperties(libxrandr)
  if (NOT libxrandr_POPULATED) # Have we downloaded libxrandr yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(libxrandr)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    FetchContent_MakeAvailable(libxrandr)
    # add_subdirectory(${libxrandr_SOURCE_DIR} ${libxrandr_BINARY_DIR})
  endif()
endif()

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


add_executable(R-Type
  ${INCLUDE_DIR}
  ${SRC_DIR}
)

target_link_libraries(R-Type
    PRIVATE
    libxrandr
    raylib
)
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/ressources/") # Set the asset path macro to the absolute path on the dev machine
install(TARGETS R-Type)
