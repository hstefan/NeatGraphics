#include <iostream>
#include <thread>
#include <array>
#include <string>
#include <fstream>
#include <streambuf>
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"

std::string get_file_contents(std::string filename) {
  auto Fin = std::ifstream{ filename, std::ios::in | std::ios::binary };
  std::string Contents;
  Fin.seekg(0, std::ios::end);
  Contents.resize(static_cast<unsigned>(Fin.tellg()));
  Fin.seekg(0, std::ios::beg);
  Fin.read(&Contents[0], Contents.size());
  Fin.close();
  return Contents;
}

GLuint make_triangle() {
  static auto Vertices = std::array<float, 6> {{ 0.0f, 0.5f,
                                                 0.5f, -0.5f,
                                                 -0.5f, -0.5f }};
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), &Vertices[0], GL_STATIC_DRAW);
  return VBO;
}

bool check_and_report_shader_compile(const GLuint shader) {
  GLint Status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &Status);
  if (Status == GL_FALSE) {
    auto MsgBuffer = std::array<char, 512>();
    glGetShaderInfoLog(shader, MsgBuffer.size(), nullptr, &MsgBuffer[0]);
    std::cerr << &MsgBuffer[0] << std::endl;
  }
  return Status == GL_TRUE;
}

GLuint make_vertex_shader(std::string sourcePath) {
  auto VertexShader = glCreateShader(GL_VERTEX_SHADER);
  const auto ShaderSource = get_file_contents(sourcePath);
  const auto ShaderContent = ShaderSource.c_str();

  glShaderSource(VertexShader, 1, &ShaderContent, nullptr);
  glCompileShader(VertexShader);

  check_and_report_shader_compile(VertexShader);

  return VertexShader;
}

GLuint make_fragment_shader(std::string sourcePath) {
  auto FragShader = glCreateShader(GL_FRAGMENT_SHADER);
  const auto ShaderSource = get_file_contents(sourcePath);
  const auto ShaderContent = ShaderSource.c_str();
  glShaderSource(FragShader, 1, &ShaderContent, nullptr);
  glCompileShader(FragShader);

  check_and_report_shader_compile(FragShader);
  
  return FragShader;
}

GLuint make_program(GLuint vertexShader, GLuint fragmentShader) {
  auto ShaderProgram = glCreateProgram();
  glAttachShader(ShaderProgram, vertexShader);
  glAttachShader(ShaderProgram, fragmentShader);
  
  glBindFragDataLocation(ShaderProgram, 0, "outColor");
  glLinkProgram(ShaderProgram);
  
  GLint Status;
  glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Status);
  if (Status == GL_FALSE) {
    auto MsgBuffer = std::array<char, 512>();
    glGetProgramInfoLog(ShaderProgram, MsgBuffer.size(), nullptr, &MsgBuffer[0]);
    std::cerr << &MsgBuffer[0] << std::endl;
  }

  auto PosAttrib = glGetAttribLocation(ShaderProgram, "position");
  glVertexAttribPointer(PosAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(PosAttrib);

  return ShaderProgram;
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

  const auto TriangleVBO = make_triangle();
  const auto VertexShader = make_vertex_shader("resources/vertex.glsl");
  const auto FragShader = make_fragment_shader("resources/frag.glsl");
  const auto ShaderProgram = make_program(VertexShader, FragShader);
  
  glBindBuffer(GL_ARRAY_BUFFER, TriangleVBO);
  glUseProgram(ShaderProgram);


  while (!glfwWindowShouldClose(Window)) {
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(Window);
    glfwPollEvents();

    if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(Window, GL_TRUE);
    }
  }

  glDeleteProgram(ShaderProgram);
  glDeleteShader(VertexShader);
  glDeleteShader(FragShader);
  glDeleteBuffers(1, &TriangleVBO);
  glDeleteVertexArrays(1, &VAO);

  glfwTerminate();

  return 0;
}
