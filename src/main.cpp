#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "class/Model.h"

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
  Shader outlineShaderProgram("../resources/shaders/outlining.vert", "../resources/shaders/outlining.frag");
  glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
  glm::mat4 lightModel = glm::mat4(1.0f);
  lightModel = glm::translate(lightModel, lightPos);

  shaderProgram.Activate();
  glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
  glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

  // Double Buffering
  glClearColor(0.85f, 0.85f, 0.90f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers(window);

  Camera camera(width, height, glm::vec3(0.0f, 0.5f, 2.0f));

  Model model("../resources/models/crow/scene.gltf");
  Model outline("../resources/models/crow-outline/scene.gltf");

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  while (!glfwWindowShouldClose(window))
  {
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    camera.Inputs(window);
    camera.updateMatrix(45.0f, 0.1f, 100.0f);

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

    model.Draw(shaderProgram, camera);

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);

    outlineShaderProgram.Activate();
    outline.Draw(outlineShaderProgram, camera);

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  shaderProgram.Delete();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
