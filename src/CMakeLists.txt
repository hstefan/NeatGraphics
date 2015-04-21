set(SOURCE
    main.cpp
    vec.cpp
    gl/ArrayBuffer.cpp
    gl/ShaderProgram.cpp
    )

set(HEADER
    mat.hpp
    vec.hpp
    gl/ArrayBuffer.hpp
    gl/GLDebug.hpp
    gl/ShaderProgram.hpp
    )

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE} ${HEADER})
target_link_libraries(${CMAKE_PROJECT_NAME}  ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} gl3w)
