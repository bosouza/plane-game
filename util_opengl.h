#ifndef UTIL_OPENGL_H_DEFINED
#define UTIL_OPENGL_H_DEFINED
#include <bits/stdc++.h>
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

unsigned int CreateShader(unsigned int type, const char *path);
std::string testCompileProgram(unsigned int program);

#endif