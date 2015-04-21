set(SOURCE
    main.cpp
    vec.cpp
    ArrayBuffer.cpp
    )

set(HEADER
    mat.hpp
    vec.hpp
    ArrayBuffer.hpp
    GLDebug.hpp
    )

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE} ${HEADER})
target_link_libraries(${CMAKE_PROJECT_NAME}  ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} gl3w)
