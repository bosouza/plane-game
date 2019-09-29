#include <bits/stdc++.h>
#include <math.h>
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "parallax_loader.h"
#define LENGTH 1000
#define HEIGHT 500
#define FPS_MIN 0.01f

#include <util_opengl.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

float speedAngle[] = {0, 3.1415f};
int action = 0;
bool actionUp = false;
bool actionDown = false;

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        speedAngle[0] += 0.01;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        speedAngle[0] -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        speedAngle[1] += 0.01;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        speedAngle[1] -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        if (!actionUp)
        {
            actionUp = true;
            action++;
        }
    }
    else
        actionUp = false;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        if (!actionDown)
        {
            actionDown = true;
            action--;
        }
    }
    else
        actionDown = false;
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
        {path : "./textures/layers/parallax-mountain-bg.png",
         parallaxRatex : 0.0f,
         parallaxRatey : 0.0f,
         stretch : 1.0f},
        {path : "./textures/layers/parallax-mountain-mountain-far.png",
         parallaxRatex : 0.01f,
         parallaxRatey : 0.005f,
         stretch : 1.0f},
        {path : "./textures/layers/parallax-mountain-mountains.png",
         parallaxRatex : 0.1f,
         parallaxRatey : 0.05f,
         stretch : 2.0f},
        {path : "./textures/layers/parallax-mountain-trees.png",
         parallaxRatex : 0.5f,
         parallaxRatey : 0.25f,
         stretch : 2.0f},
        {path : "./textures/layers/parallax-mountain-foreground-trees.png",
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

    float verticesLeft[] = {
        // positions
        0.0f, 1.0f, 0.0f,   // top right
        0.0f, -1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f, // bottom left
        -1.0f, 1.0f, 0.0f,  // top left
    };
    float verticesRight[] = {
        // positions
        1.0f, 1.0f, 0.0f,  // top right
        1.0f, -1.0f, 0.0f, // bottom right
        0.0f, -1.0f, 0.0f, // bottom left
        0.0f, 1.0f, 0.0f,  // top left
    };
    float leftPlane[] = {
        // positions
        -0.4f, -0.3f, 0.0f, // top right
        -0.4f, -0.6f, 0.0f, // bottom right
        -0.6f, -0.6f, 0.0f, // bottom left
        -0.6f, -0.3f, 0.0f, // top left
    };
    float rightPlane[] = {
        // positions
        0.6f, -0.3f, 0.0f, // top right
        0.6f, -0.6f, 0.0f, // bottom right
        0.4f, -0.6f, 0.0f, // bottom left
        0.4f, -0.3f, 0.0f, // top left
    };

    unsigned int leftVBO;
    glGenBuffers(1, &leftVBO);
    glBindBuffer(GL_ARRAY_BUFFER, leftVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLeft), verticesLeft, GL_STATIC_DRAW);
    unsigned int rightVBO;
    glGenBuffers(1, &rightVBO);
    glBindBuffer(GL_ARRAY_BUFFER, rightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRight), verticesRight, GL_STATIC_DRAW);

    unsigned int leftPlaneVBO;
    glGenBuffers(1, &leftPlaneVBO);
    glBindBuffer(GL_ARRAY_BUFFER, leftPlaneVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(leftPlane), leftPlane, GL_STATIC_DRAW);
    unsigned int rightPlaneVBO;
    glGenBuffers(1, &rightPlaneVBO);
    glBindBuffer(GL_ARRAY_BUFFER, rightPlaneVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rightPlane), rightPlane, GL_STATIC_DRAW);

    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    for (int i = 0; i < 2; i++)
    {
        for (auto &image : images)
        {
            glGenVertexArrays(1, &image.VAOs[i]);
            glBindVertexArray(image.VAOs[i]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

            // position attribute
            glBindBuffer(GL_ARRAY_BUFFER, i == 0 ? leftVBO : rightVBO);
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
    }

    sprite redPlaneSprite("./textures/red-plane-sprite.png", 5, 5);

    float buffer[8];
    redPlaneSprite.FillTextureBuffer(buffer);
    unsigned int planeVAOs[2];
    for (int i = 0; i < 2; i++)
    {
        glGenVertexArrays(1, &planeVAOs[i]);
        glBindVertexArray(planeVAOs[i]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        // position attribute
        glBindBuffer(GL_ARRAY_BUFFER, i == 0 ? leftPlaneVBO : rightPlaneVBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // texture attribute
        unsigned int textureVBO;
        glGenBuffers(1, &textureVBO);
        glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(1);
    }

    float speed[] = {0.002, 0.002};
    float positionx[] = {0, 0}, positiony[] = {0, 0};
    double previousSeconds = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        double currentSeconds = glfwGetTime();
        double elapsedSeconds = currentSeconds - previousSeconds;
        if (elapsedSeconds >= FPS_MIN)
        {
            previousSeconds = currentSeconds;
            processInput(window);
            for (int i = 0; i < 2; i++)
            {
                positionx[i] += speed[i] * cos(speedAngle[i]);
                positiony[i] += speed[i] * sin(speedAngle[i]);
                if (positiony[i] < 0)
                {
                    positiony[i] = 0;
                }
            }

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            for (parallaxImage image : images)
            {
                for (int i = 0; i < 2; i++)
                {
                    glBindVertexArray(image.VAOs[i]);
                    glBindTexture(GL_TEXTURE_2D, image.textureId);

                    glUniform1f(glGetUniformLocation(shaderProgram, "offsetx"), positionx[i] * image.parallaxRatex / image.stretch);
                    glUniform1f(glGetUniformLocation(shaderProgram, "offsety"), positiony[i] * image.parallaxRatey / image.stretch);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }
            }
            for (int i = 0; i < 2; i++)
            {
                glBindVertexArray(planeVAOs[i]);
                redPlaneSprite.BindAction(action, glGetUniformLocation(shaderProgram, "offsetx"), glGetUniformLocation(shaderProgram, "offsety"));
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}