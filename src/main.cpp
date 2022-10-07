#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <fstream>
#include <string>

using namespace std;

string loadFile(const char *);

int main()
{

  glfwInit();
  glfwDefaultWindowHints();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  string vertShaderContent = loadFile("../src/shaders/vertexShader.vert");
  const char *vertShaderSource = vertShaderContent.c_str();

  string fragShaderContent = loadFile("../src/shaders/fragmentShader.frag");
  const char *fragShaderSource = fragShaderContent.c_str();

  GLfloat vertices[] = {
      -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
      0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
      0.0f, 0.5f * float(sqrt(3)) / 3, 0.0f};

  GLFWwindow *window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);


  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  gladLoadGL();

  // glViewport(0, 0, 800, 800);
  // General Display

  int pWidth, pHeight;

  glfwGetFramebufferSize(window, &pWidth, &pHeight);
  glViewport(0, 0, pWidth, pHeight);

  // Mac (With Retina Display)

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertShaderSource, NULL);
  glCompileShader(vertexShader);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
  glCompileShader(fragmentShader);

  GLuint shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  glLinkProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  GLuint VAO, VBO; // Vertex buffer Array Object, Vertex Buffer Object

  glGenVertexArrays(1, &VAO); // Must declare VAO first and VBO
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Optional
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Double Buffering
  glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers(window);

  while (!glfwWindowShouldClose(window))
  {
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

string loadFile(const char *path)
{
  string content;
  ifstream file(path);

  string str;

  if (!file.is_open())
  {
    cout << "Cannot open " << path << " : " << strerror(errno) << endl;
  }
  while (getline(file, str))
  {
    content.append(str + "\n");
  }

  file.close();
  return content;
}