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
const unsigned int numWindows = 100;
glm::vec3 positionsWin[numWindows];
float rotationsWin[numWindows];

unsigned int orderDraw[numWindows];
float distanceCamera[numWindows];

int compare(const void *a, const void *b)
{
  double diff = distanceCamera[*(int *)b] - distanceCamera[*(int *)a];
  return (0 < diff) - (diff < 0);
}

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
  Shader grassShader("../resources/shaders/vertexShader.vert", "../resources/shaders/grassShader.frag");
  Shader winShader("../resources/shaders/vertexShader.vert", "../resources/shaders/windows.frag");

  glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
  glm::mat4 lightModel = glm::mat4(1.0f);
  lightModel = glm::translate(lightModel, lightPos);

  shaderProgram.Activate();
  glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
  glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

  grassShader.Activate();
  glUniform4f(glGetUniformLocation(grassShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
  glUniform3f(glGetUniformLocation(grassShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

  winShader.Activate();
  glUniform4f(glGetUniformLocation(winShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
  glUniform3f(glGetUniformLocation(winShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
  // Double Buffering
  glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers(window);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

  Model grass("../resources/models/grass/scene.gltf");
  Model ground("../resources/models/ground/scene.gltf");
  Model windows("../resources/models/windows/scene.gltf");

  glEnable(GL_DEPTH_TEST);

  for (unsigned int i = 0; i < numWindows; i++)
  {
    positionsWin[i] = glm::vec3(
        -15.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (15.0f - (-15.0f)))),
        1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (4.0f - (1.0f)))),
        -15.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (15.0f - (-15.0f)))));
    rotationsWin[i] = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 1.0f));
    orderDraw[i] = i;
  }

  while (!glfwWindowShouldClose(window))
  {
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.Inputs(window);
    camera.updateMatrix(45.0f, 0.1f, 100.0f);

    grass.Draw(grassShader, camera);
    ground.Draw(shaderProgram, camera);

    glEnable(GL_BLEND);
    for (unsigned int i = 0; i < numWindows; i++)
    {
      distanceCamera[i] = glm::length(camera.Position - positionsWin[i]);
    }

    qsort(orderDraw, numWindows, sizeof(unsigned int), compare);

    for (unsigned int i = 0; i < numWindows; i++)
    {
      windows.Draw(winShader, camera, positionsWin[orderDraw[i]], glm::quat(1.0f, 0.0f, rotationsWin[orderDraw[i]], 0.0f));
    }

    glDisable(GL_BLEND);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  shaderProgram.Delete();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
