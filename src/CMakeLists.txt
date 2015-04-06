set(SOURCE
  main.cpp
)

set(HEADER
  mat.hpp
)

add_executable(NeatGraphics ${SOURCE} ${HEADER})
target_link_libraries(NeatGraphics  ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} gl3w)
