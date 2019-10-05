#ifndef UTIL_OPENGL_H_DEFINED
#define UTIL_OPENGL_H_DEFINED
#include <string>

unsigned int CreateShader(unsigned int type, const char *path);
std::string testCompileProgram(unsigned int program);
void fillRectangleBuffer(float width, float height, float buffer[]);
#endif