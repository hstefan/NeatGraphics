#include <iostream>
#include <array>
#include <string>
#include <fstream>
#include <streambuf>
#include <cmath>
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include "mat.hpp"
#include "vec.hpp"
#include "gl/ArrayBuffer.hpp"
#include "gl/ShaderProgram.hpp"
#include "gl/Shader.hpp"

using gl::ArrayBuffer;
using gl::ShaderProgram;
using gl::Shader;

std::string get_file_contents(std::string filename) {
    auto fin = std::ifstream{filename, std::ios::in | std::ios::binary};
    std::string contents;
    fin.seekg(0, std::ios::end);
    contents.resize(static_cast<unsigned>(fin.tellg()));
    fin.seekg(0, std::ios::beg);
    fin.read(&contents[0], contents.size());
    fin.close();
    return contents;
}

ArrayBuffer make_triangle() {
    static auto vertices = std::array<float, 20>{{
        -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,   // top-left
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f,    // top-right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f,   // bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  // bottom-left
    }};
    return ArrayBuffer{vertices};
}

Shader make_shader(GLenum shaderType, std::string sourcePath) {
    const auto shaderSource = get_file_contents(sourcePath);
    auto shader = Shader{shaderType, shaderSource};
    return shader;
}

ShaderProgram make_program(GLuint vertexShader, GLuint fragmentShader) {
    auto shaderProgram = ShaderProgram{};
    shaderProgram.attachShader(vertexShader);
    shaderProgram.attachShader(fragmentShader);
    shaderProgram.bindFragDataLocation(0, "outColor");
    shaderProgram.link();

    auto posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          0);

    auto colorAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colorAttrib);
    glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)(2 * sizeof(float)));
    return shaderProgram;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    auto window = glfwCreateWindow(800, 600, "OpenGL Test", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if (gl3wInit()) {
        std::cerr << "Failed to initialize OpenGL" << std::endl;
        return -1;
    }

    if (!gl3wIsSupported(3, 2)) {
        std::cerr << "OpenGL 3.2 not supported" << std::endl;
        return -1;
    }

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    auto triangleVBO = make_triangle();
    const auto vertexShader =
        make_shader(GL_VERTEX_SHADER, "resources/vertex.glsl");
    const auto fragShader =
        make_shader(GL_FRAGMENT_SHADER, "resources/frag.glsl");
    const auto shaderProgram = make_program(vertexShader, fragShader);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    static std::array<unsigned, 6> Elements{{0, 1, 2, 2, 3, 0}};

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Elements), Elements.data(),
                 GL_STATIC_DRAW);

    triangleVBO.bind();
    shaderProgram.use();

    // clang-format off
    math::mat4 transform{
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f,
    };
    // clang-format on

    auto transformUni = shaderProgram.getUniformLocation("transform");
    glUniformMatrix4fv(transformUni, 1, GL_FALSE, transform.data());

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }

    glDeleteVertexArrays(1, &vao);

    glfwTerminate();

    return 0;
}
