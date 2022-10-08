#include "shaders.h"

std::string loadFile(const char *path)
{
  std::string content;
  std::ifstream file(path);

  std::string str;

  if (!file.is_open())
  {
    std::cout << "Cannot open " << path << " : " << strerror(errno) << std::endl;
  }
  while (getline(file, str))
  {
    content.append(str + "\n");
  }

  file.close();
  return content;
}

Shader::Shader(const char *vertexFile, const char *fragFile)
{
  std::string vertShaderContent = loadFile(vertexFile);
  const char *vertShaderSource = vertShaderContent.c_str();

  std::string fragShaderContent = loadFile(fragFile);
  const char *fragShaderSource = fragShaderContent.c_str();

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertShaderSource, NULL);
  glCompileShader(vertexShader);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
  glCompileShader(fragmentShader);

  ID = glCreateProgram();

  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);

  glLinkProgram(ID);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
  glUseProgram(ID);
}

void Shader::Delete()
{
  glDeleteProgram(ID);
}