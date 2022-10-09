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
    /*  -0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,  // Bottom side
      -0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 5.0f, 0.0f, -1.0f, 0.0f, // Bottom side
      0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 5.0f, 0.0f, -1.0f, 0.0f,  // Bottom side
      0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f,   // Bottom side

      -0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f, -0.8f, 0.5f, 0.0f,  // Left side
      -0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f, -0.8f, 0.5f, 0.0f, // Left side
      0.0f, 0.8f, 0.0f, 0.92f, 0.86f, 0.76f, 2.5f, 5.0f, -0.8f, 0.5f, 0.0f,   // Left side

      -0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f, 0.0f, 0.5f, -0.8f, // Non-Facing side
      0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f, 0.0f, 0.5f, -0.8f,  // Non-Facing side
      0.0f, 0.8f, 0.0f, 0.92f, 0.86f, 0.76f, 2.5f, 5.0f, 0.0f, 0.5f, -0.8f,   // Non-Facing side

      0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f, 0.8f, 0.5f, 0.0f, // Right side
      0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f, 0.8f, 0.5f, 0.0f,  // Right side
      0.0f, 0.8f, 0.0f, 0.92f, 0.86f, 0.76f, 2.5f, 5.0f, 0.8f, 0.5f, 0.0f,  // Right side

      0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f, 0.0f, 0.5f, 0.8f,  // Facing side
      -0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f, 0.0f, 0.5f, 0.8f, // Facing side
      0.0f, 0.8f, 0.0f, 0.92f, 0.86f, 0.76f, 2.5f, 5.0f, 0.0f, 0.5f, 0.8f,  // Facing side */
    -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  //
    -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, //
    1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,  //
    1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   //
};

GLuint indices[] =
    {
        /*0, 1, 2,
        0, 2, 3,
        4, 6, 5,
        7, 9, 8,
        10, 12, 11,
        13, 15, 14*/
        0, 1, 2,
        0, 2, 3};

GLfloat lightVerticies[] =
    {
        -0.1f, -0.1f, 0.1f,
        -0.1f, -0.1f, -0.1f,
        0.1f, -0.1f, -0.1f,
        0.1f, -0.1f, 0.1f,
        -0.1f, 0.1f, 0.1f,
        -0.1f, 0.1f, -0.1f,
        0.1f, 0.1f, -0.1f,
        0.1f, 0.1f, 0.1f};

GLuint lightIndicies[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 4, 7,
        0, 7, 3,
        3, 7, 6,
        3, 6, 2,
        2, 6, 5,
        2, 5, 1,
        1, 5, 4,
        1, 4, 0};

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

  VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void *)0);
  VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void *)(3 * sizeof(float)));
  VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void *)(6 * sizeof(float)));
  VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void *)(8 * sizeof(float)));

  VAO1.Unbind();
  VBO1.Unbind();
  EBO1.Unbind();

  Shader lightShader("../resources/shaders/light.vert", "../resources/shaders/light.frag");
  VAO lightVAO;
  lightVAO.Bind();

  VBO lightVBO(lightVerticies, sizeof(lightVerticies));
  EBO lightEBO(lightIndicies, sizeof(lightIndicies));

  lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void *)0);

  lightVAO.Unbind();
  lightVBO.Unbind();
  lightEBO.Unbind();

  glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
  glm::mat4 lightModel = glm::mat4(1.0f);
  lightModel = glm::translate(lightModel, lightPos);

  glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::mat4 pyramidModel = glm::mat4(1.0f);
  pyramidModel = glm::translate(pyramidModel, pyramidPos);

  lightShader.Activate();
  glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
  glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
  shaderProgram.Activate();
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
  glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
  glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

  GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

  Texture popcat("../resources/textures/planks.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
  popcat.texUnit(shaderProgram, "tex0", 0);

  Texture planksSpec("../resources/textures/planksSpec.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);
  planksSpec.texUnit(shaderProgram, "tex1", 1);

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

    camera.Inputs(window);
    camera.updateMatrix(45.0f, 0.1f, 100.0f);

    shaderProgram.Activate();
    camera.Matrix(shaderProgram, "camMatrix");
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    glUniform1f(uniID, 0.0f);
    popcat.Bind();
    planksSpec.Bind();
    VAO1.Bind();
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

    lightShader.Activate();
    camera.Matrix(lightShader, "camMatrix");
    lightVAO.Bind();
    glDrawElements(GL_TRIANGLES, sizeof(lightIndicies) / sizeof(int), GL_UNSIGNED_INT, 0);

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
