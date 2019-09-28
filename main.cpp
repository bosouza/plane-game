#include <bits/stdc++.h>
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "parallax_loader.h"
#define LENGTH 1000
#define HEIGHT 500

#include <util_opengl.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

float speedx = 0, speedy = 0;

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        speedy += 0.00001;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        speedx -= 0.00001;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        speedy -= 0.00001;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        speedx += 0.00001;
}

int main()
{
    if (!glfwInit())
    {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(LENGTH, HEIGHT, "Simple translation exercise", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    glViewport(0, 0, LENGTH, HEIGHT);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    unsigned int vertexShader = CreateShader(GL_VERTEX_SHADER, "./simpleVertexShader.vert");

    unsigned int fragmentShader = CreateShader(GL_FRAGMENT_SHADER, "./simpleFragShader.frag");

    parallaxImage images[] = {
        {path : "./layers/parallax-mountain-bg.png",
         parallaxRatex : 0.0f,
         parallaxRatey : 0.0f,
         stretch : 1.0f},
        {path : "./layers/parallax-mountain-mountain-far.png",
         parallaxRatex : 0.01f,
         parallaxRatey : 0.005f,
         stretch : 1.0f},
        {path : "./layers/parallax-mountain-mountains.png",
         parallaxRatex : 0.1f,
         parallaxRatey : 0.05f,
         stretch : 2.0f},
        {path : "./layers/parallax-mountain-trees.png",
         parallaxRatex : 0.5f,
         parallaxRatey : 0.25f,
         stretch : 2.0f},
        {path : "./layers/parallax-mountain-foreground-trees.png",
         parallaxRatex : 1.0f,
         parallaxRatey : 1.0f,
         stretch : 2.0f}};
    if (!LoadImages(images, 5))
    {
        glfwTerminate();
        return -1;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    cout << testCompileProgram(shaderProgram);

    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
        // positions  // texture coords
        0.0f, 1.0f, 0.0f,   // top right
        0.0f, -1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f, // bottom left
        -1.0f, 1.0f, 0.0f,  // top left
    };
    unsigned int positionVBO;
    glGenBuffers(1, &positionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    for (auto &image : images)
    {
        glGenVertexArrays(1, &image.VAO);
        glBindVertexArray(image.VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        // position attribute
        glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // texture attribute
        unsigned int textureVBO;
        glGenBuffers(1, &textureVBO);
        glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(image.maping), image.maping, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(1);
    }

    float positionx = 0, positiony = 0;
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        positionx += speedx;
        positiony += speedy;
        if (positiony < 0)
        {
            positiony = 0;
            speedy = 0;
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (parallaxImage image : images)
        {
            glBindVertexArray(image.VAO);
            glBindTexture(GL_TEXTURE_2D, image.textureId);

            glUniform1f(glGetUniformLocation(shaderProgram, "offsetx"), positionx * image.parallaxRatex / image.stretch);
            glUniform1f(glGetUniformLocation(shaderProgram, "offsety"), positiony * image.parallaxRatey / image.stretch);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}