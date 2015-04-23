set(SOURCE
    main.cpp
    math/vec.cpp
    gl/ArrayBuffer.cpp
    gl/ShaderProgram.cpp
    gl/Shader.cpp
    )

set(HEADER
    math/mat.hpp
    math/vec.hpp
    math/transform.hpp
    util/NonCopyable.hpp
    gl/ArrayBuffer.hpp
    gl/GLDebug.hpp
    gl/ShaderProgram.hpp
    gl/Shader.hpp
    )

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE} ${HEADER})
target_link_libraries(${CMAKE_PROJECT_NAME}  ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} gl3w)
