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
#include "ArrayBuffer.hpp"
#include "ShaderProgram.hpp"
#include "GLDebug.hpp"

std::string get_file_contents(std::string filename) {
    auto Fin = std::ifstream{filename, std::ios::in | std::ios::binary};
    std::string Contents;
    Fin.seekg(0, std::ios::end);
    Contents.resize(static_cast<unsigned>(Fin.tellg()));
    Fin.seekg(0, std::ios::beg);
    Fin.read(&Contents[0], Contents.size());
    Fin.close();
    return Contents;
}

cg::ArrayBuffer make_triangle() {
    static auto Vertices = std::array<float, 20>{{
        -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,  // top-left
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  // top-right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f,  // bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  // bottom-left
    }};
    return cg::ArrayBuffer{Vertices};
}

bool check_and_report_shader_compile(const GLuint shader) {
    GLint Status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &Status);
    if (Status == GL_FALSE) {
        auto MsgBuffer = std::array<char, 512>();
        glGetShaderInfoLog(shader, MsgBuffer.size(), nullptr, &MsgBuffer[0]);
        std::cerr << MsgBuffer.data() << std::endl;
    }
    return Status == GL_TRUE;
}

GLuint make_shader(GLenum shaderType, std::string sourcePath) {
    auto Shader = glCreateShader(shaderType);
    const auto ShaderSource = get_file_contents(sourcePath);
    const auto ShaderContent = ShaderSource.c_str();

    glShaderSource(Shader, 1, &ShaderContent, nullptr);
    glCompileShader(Shader);

    check_and_report_shader_compile(Shader);

    return Shader;
}

cg::ShaderProgram make_program(GLuint vertexShader, GLuint fragmentShader) {
    auto shaderProgram = cg::ShaderProgram{};
    shaderProgram.attachShader(vertexShader);
    shaderProgram.attachShader(fragmentShader);
    shaderProgram.bindFragDataLocation(0, "outColor");
    shaderProgram.link();

    auto PosAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(PosAttrib);
    glVertexAttribPointer(PosAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          0);

    auto ColorAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(ColorAttrib);
    glVertexAttribPointer(ColorAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
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

    auto Window = glfwCreateWindow(800, 600, "OpenGL Test", nullptr, nullptr);
    glfwMakeContextCurrent(Window);

    if (gl3wInit()) {
        std::cerr << "Failed to initialize OpenGL" << std::endl;
        return -1;
    }

    if (!gl3wIsSupported(3, 2)) {
        std::cerr << "OpenGL 3.2 not supported" << std::endl;
        return -1;
    }

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    auto TriangleVBO = make_triangle();
    const auto VertexShader =
        make_shader(GL_VERTEX_SHADER, "resources/vertex.glsl");
    const auto FragShader =
        make_shader(GL_FRAGMENT_SHADER, "resources/frag.glsl");
    const auto shaderProgram = make_program(VertexShader, FragShader);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    static std::array<unsigned, 6> Elements{{0, 1, 2, 2, 3, 0}};

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Elements), Elements.data(),
                 GL_STATIC_DRAW);

    TriangleVBO.bind();
    shaderProgram.use();

    math::mat4 Transform{
        1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f,
    };

    auto vec_i = math::vec3{1.0f, 0.0f, 0.0f};
    auto vec_j = math::vec3{0.0f, 1.0f, 0.0f};
    std::cout << math::cross(vec_i, vec_j) << std::endl;

    auto transformUni = shaderProgram.getUniformLocation("transform");
    glUniformMatrix4fv(transformUni, 1, GL_FALSE, Transform.data());

    while (!glfwWindowShouldClose(Window)) {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(Window);
        glfwPollEvents();

        if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(Window, GL_TRUE);
        }
    }

    glDeleteShader(VertexShader);
    glDeleteShader(FragShader);
    glDeleteVertexArrays(1, &VAO);

    glfwTerminate();

    return 0;
}
