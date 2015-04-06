cmake_minimum_required (VERSION 3.0)

project(NeatGraphics)

if (NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++ -lc++abi")
endif()

add_definitions(${CXX_COMPILE_FLAGS})

find_package(OpenGL REQUIRED)

option(ENABLE_GLFW "Enable GLFW frontend" ON)
if (ENABLE_GLFW)
  if (WIN32)
    set(GLFW_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/external/glfw-3.1.1")
    set(GLFW_INCLUDE_DIRS "${GLFW_PREFIX}/include")
    set(GLFW_LIBRARY_DIRS "${GLFW_PREFIX}/lib-vc2013")
    set(GLFW_LIBRARIES glfw3)
  else()
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
  endif()

  include_directories(${GLFW_INCLUDE_DIRS})
  link_directories(${GLFW_LIBRARY_DIRS})
endif()

include_directories(src)
include_directories(${CMAKE_SOURCE_DIR}/external/gl3w/include)
include_directories(${OPENGL_INCLUDE_DIR})


add_subdirectory(external)
add_subdirectory(src)
