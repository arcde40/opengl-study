#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "class/shaders.h"
#include "class/VAO.h"
#include "class/VBO.h"
#include "class/EBO.h"
#include "class/texture.h"
#include "class/camera.h"

GLfloat vertices[] = {
    -0.5f,
    0.0f,
    0.5f,
    0.83f,
    0.70f,
    0.44f,
    0.0f,
    0.0f,
    -0.5f,
    0.0f,
    -0.5f,
    0.83f,
    0.70f,
    0.44f,
    5.0f,
    0.0f,
    0.5f,
    0.0f,
    -0.5f,
    0.83f,
    0.70f,
    0.44f,
    0.0f,
    0.0f,
    0.5f,
    0.0f,
    0.5f,
    0.83f,
    0.70f,
    0.44f,
    5.0f,
    0.0f,
    0.0f,
    0.8f,
    0.0f,
    0.92f,
    0.86f,
    0.76f,
    2.5f,
    5.0f,
};

GLuint indices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4};

int width = 800, height = 800;

int main()
{

  glfwInit();
  glfwDefaultWindowHints();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);

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

  Shader shaderProgram("../resources/shaders/vertexShader.vert", "../resources/shaders/fragmentShader.frag");

  VAO VAO1;
  VAO1.Bind();

  VBO VBO1(vertices, sizeof(vertices));
  EBO EBO1(indices, sizeof(indices));

  VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);
  VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float)));

  VAO1.Unbind();
  VBO1.Unbind();
  EBO1.Unbind();

  GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

  Texture popcat("../resources/textures/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
  popcat.texUnit(shaderProgram, "tex0", 0);

  // Double Buffering
  glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers(window);

  Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
  glEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(window))
  {
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.Activate();

    camera.Inputs(window);
    camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

    glUniform1f(uniID, 0.0f);
    popcat.Bind();
    VAO1.Bind();
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  VAO1.Delete();
  VBO1.Delete();
  EBO1.Delete();
  shaderProgram.Delete();
  popcat.Delete();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
