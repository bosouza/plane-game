#include <util_opengl.h>

std::string readFile(const char *filePath)
{
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open())
    {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

std::string testCompileShader(unsigned int shader)
{
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::ostringstream stringStm;
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        stringStm << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        return stringStm.str();
    }
    return "";
}

std::string testCompileProgram(unsigned int program)
{
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::ostringstream stringStm;
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        stringStm << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        return stringStm.str();
    }
    return "";
}

unsigned int CreateShader(unsigned int type, const char *path)
{
    std::string src = readFile(path);
    const char *src_ptr = src.c_str();
    unsigned int shaderID;
    shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, &src_ptr, NULL);
    glCompileShader(shaderID);
    std::cout << testCompileShader(shaderID);
    return shaderID;
}

void fillRectangleBuffer(float width, float height, float buffer[])
{
    float bufferi[] = {width / 2.0f, height / 2.0f, 0.0f,
                       width / 2.0f, -height / 2.0f, 0.0f,
                       -width / 2.0f, -height / 2.0f, 0.0f,
                       -width / 2.0f, height / 2.0f, 0.0f};
    for (int i = 0; i < 12; i++)
        buffer[i] = bufferi[i];
}