#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cerrno>

std::string loadFile(const char *);

class Shader
{
public:
  GLuint ID;
  Shader(const char *vertexFile, const char *fragFile);

  void Activate();
  void Delete();
};

#endif