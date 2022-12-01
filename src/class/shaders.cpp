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

Shader::Shader(const char *vertexFile, const char *fragFile, const char *geometryFile)
{
  std::string vertShaderContent = loadFile(vertexFile);
  const char *vertShaderSource = vertShaderContent.c_str();

  std::string fragShaderContent = loadFile(fragFile);
  const char *fragShaderSource = fragShaderContent.c_str();

  std::string geoShaderContent = loadFile(geometryFile);
  const char *geoShaderSource = geoShaderContent.c_str();

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertShaderSource, NULL);
  glCompileShader(vertexShader);
  compileErrors(vertexShader, vertexFile);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
  glCompileShader(fragmentShader);
  compileErrors(fragmentShader, fragFile);

  GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
  glShaderSource(geometryShader, 1, &geoShaderSource, NULL);
  glCompileShader(geometryShader);
  compileErrors(geometryShader, geometryFile);

  ID = glCreateProgram();

  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  glAttachShader(ID, geometryShader);

  glLinkProgram(ID);
  compileErrors(ID, "PROGRAM");

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteShader(geometryShader);
}

void Shader::Activate()
{
  glUseProgram(ID);
}

void Shader::Delete()
{
  glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char *type)
{
  GLint hasCompiled;
  char infoLog[1024];
  if (strcmp(type, "PROGRAM"))
  {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
    if (hasCompiled == GL_FALSE)
    {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "SHADER_COMPILATION_ERROR for: " << type << "\n"
                << infoLog
                << std::endl;
    }
  }
  else
  {
    glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
    if (hasCompiled == GL_FALSE)
    {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "SHADER_LINKING_ERROR for: " << type << "\n"
                << infoLog
                << std::endl;
    }
  }
}
