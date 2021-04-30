cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

# Set project name and version
project(chess VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

# Enable debug symbols by default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug 
    CACHE STRING 
      "Choose the type of build (Debug or Release)" FORCE)
endif()

# Export compile commands for completion engines (optional)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set option to control setting the resource path variable
option(USE_INSTALL_RESOURCE_PATH "Set resource path to install location" OFF)

if(NOT USE_INSTALL_RESOURCE_PATH)
  set(RESOURCE_PATH "${PROJECT_SOURCE_DIR}/content/")
else()
  set(RESOURCE_PATH "${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROJECT_NAME}/content/")
endif()

# Generate config.h
configure_file(include/config.h.in config.h)

#not needed
# Find SFML shared libraries
# find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

# Download SFML submodule
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}.git")
  # Update submodule as needed
  option(GIT_SUBMODULE "Check submodules during build" ON)
  if(GIT_SUBMODULE)
    message(STATUS "Submoduel update")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
      message(FATAL_ERROR "git submodule update --init fialed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()
  endif()
endif()

# Check all the submodules
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/sfml/CMakeLists.txt")
  message(FATAL_ERROR "The sfml was not downloaded! GIT_SUBMODULE was turned off or failed")
endif()

# Add SFML submodule
add_subdirectory(external/sfml)

# Compile executable
add_executable(${PROJECT_NAME} src/main.cpp src/Moves.cpp src/Piece.cpp)

# Set include directory search paths
target_include_directories(${PROJECT_NAME} 
  PRIVATE
    "${PROJECT_BINARY_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
  PUBLIC
    external/sfml/include
  )

# Set executable RPATH
set_target_properties(chess
  PROPERTIES
      INSTALL_RPATH
        "${CMAKE_INSTALL_PREFIX}/lib"
  )

target_link_directories(${PROJECT_NAME}
  PRIVATE external/sfml/src
)

# Link executable to required SFML modules
target_link_libraries(chess sfml-graphics)

# Set executable install location
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

install(FILES "${PROJECT_BINARY_DIR}/config.h" DESTINATION include)

# Set install location for resources
install(DIRECTORY content
  DESTINATION "${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROJECT_NAME}"
)

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${chess_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${chess_VERSION_MINOR}")
include(CPack)