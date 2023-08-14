cmake_minimum_required(VERSION 3.15)
# Global Variables
set(TARGET_NAME yoyoTetris)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(${TARGET_NAME} VERSION 1.0 LANGUAGES C CXX)

# Raylib
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

# Add the RayGui library
set(RAYGUI_VERSION 3.6) 
find_package(raygui ${RAYGUI_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raygui_FOUND) # If there's none, fetch and build raygui
  include(FetchContent)
  FetchContent_Declare(
    raygui
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raygui/archive/refs/tags/${RAYGUI_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raygui)
  if (NOT raygui_POPULATED) # Have we downloaded raygui yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raygui)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    add_subdirectory(${raygui_SOURCE_DIR}/projects/CMake)
    #add_subdirectory(${raygui_SOURCE_DIR} ${raygui_BINARY_DIR})
  endif()
endif()

#Setting up the sources directory
file(GLOB SOURCES
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        )

# yoyoTetris Project (source, header)
add_executable(${TARGET_NAME} ${SOURCES})
target_include_directories(${TARGET_NAME} PUBLIC "include")

# Adding Librairies
target_link_libraries(${PROJECT_NAME} raylib raygui)

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

# Adding compilation option depending on the OS 
if(MSVC)
    target_compile_options(${TARGET_NAME} PUBLIC /W3 /WX /DEBUG "-Wno-missing-field-initializers" "-Wno-unused-parameter" "-Wno-unused-variable" "-Wno-unused-but-set-variable" "-Wno-deprecated-enum-compare-conditional")
else()
    target_compile_options(${TARGET_NAME} PUBLIC -Wall -Wextra -Wpedantic -Werror "-Wno-missing-field-initializers" "-Wno-unused-parameter" "-Wno-unused-variable" "-Wno-unused-but-set-variable" "-Wno-deprecated-enum-compare-conditional")
endif()