cmake_minimum_required(VERSION 3.25)
# Global Variables
set(TARGET_NAME yoyoTetris)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Set variables for versionning
add_definitions( -D_VERSION_MAJOR=\"1\")
add_definitions( -D_VERSION_MINOR=\"0\")
add_definitions( -D_VERSION_PATCH=\"3\")
project(${TARGET_NAME} VERSION 1.0.3 LANGUAGES C CXX)

# Raylib
set(RAYLIB_VERSION 5.5)
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

#Setting up the sources directory
file(GLOB BASE "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB GAME "${PROJECT_SOURCE_DIR}/src/*/*.cpp")

# yoyoTetris Project (source, header)
add_executable(${TARGET_NAME} ${BASE} ${GAME})
target_include_directories(${TARGET_NAME} PUBLIC "include")

# Adding Librairies
target_link_libraries(${PROJECT_NAME} raylib)

# Copying the ressource folder to the build
add_custom_target(CopyResources ALL
      COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/res
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/res
      COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res
        ${CMAKE_BINARY_DIR}/res
    COMMENT "Copying and deleting resources to build directory"
    VERBATIM
)
add_dependencies(${PROJECT_NAME} CopyResources)

# Adding the icon if it's a windows executable
if (WIN32)
set(RESOURCE_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/scripts/res.rc)
set(RESOURCE_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/res.res)
add_custom_command(
    OUTPUT ${RESOURCE_OUTPUT}
    COMMAND rc.exe /fo ${RESOURCE_OUTPUT} ${RESOURCE_SCRIPT}
    DEPENDS ${RESOURCE_SCRIPT}
    COMMENT "Compiling resource script for executable icon (Windows ONLY) on ${CMAKE_CURRENT_SOURCE_DIR}/scripts/res.rc to ${CMAKE_CURRENT_BINARY_DIR}/scripts/res.res"
)
add_custom_target(ResourceCompilation ALL DEPENDS ${RESOURCE_OUTPUT})
target_link_libraries(${PROJECT_NAME} ${RESOURCE_OUTPUT})
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

# Compilation depending on the platform
if(MSVC)
  # Build the program as a window-only (no cmd)
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_options(${TARGET_NAME} PRIVATE "/SUBSYSTEM:WINDOWS")
    target_compile_options(${TARGET_NAME} PRIVATE /W3 /WX )
  else()
    target_compile_options(${TARGET_NAME} PRIVATE /W3 /WX /DEBUG )
  endif()
else()
  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

