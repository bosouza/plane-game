#include <bits/stdc++.h>
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#define LENGTH 1000
#define HEIGHT 500
#define NUMBER_OF_PLANES 2
#define ANGULAR_VELOCITY 1
#define FPS_MAX 120

#include <util_opengl.h>
#include <button_util.h>
#include <parallax_image.h>
#include <sprite.h>
#include <game_entity.h>
#include <timer.h>

using namespace std;

unsigned int mod(int a, int b);
unsigned int actionFromAngle(float angle);
void processInput(GLFWwindow *window, game_entity planes[]);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

vector<parallax_image> images;

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

    unsigned int vertexShader = CreateShader(GL_VERTEX_SHADER, "./shaders/simpleVertexShader.vert");
    unsigned int fragmentShader = CreateShader(GL_FRAGMENT_SHADER, "./shaders/simpleFragShader.frag");

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    cout << testCompileProgram(shaderProgram);

    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    int positionxLocation = glGetUniformLocation(shaderProgram, "positionx");
    int positionyLocation = glGetUniformLocation(shaderProgram, "positiony");
    int offsetxLocation = glGetUniformLocation(shaderProgram, "offsetx");
    int offsetyLocation = glGetUniformLocation(shaderProgram, "offsety");

    float backgroundBuffer[12];
    fillRectangleBuffer(1, 2, backgroundBuffer);
    float planeBuffer[12];
    fillRectangleBuffer(0.15f, 0.2f, planeBuffer);

    unsigned int backgroundVBO;
    glGenBuffers(1, &backgroundVBO);
    glBindBuffer(GL_ARRAY_BUFFER, backgroundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundBuffer), backgroundBuffer, GL_STATIC_DRAW);

    unsigned int planeVBO;
    glGenBuffers(1, &planeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeBuffer), planeBuffer, GL_STATIC_DRAW);

    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    images.push_back(parallax_image("./textures/layers/parallax-mountain-bg.png", 0.0f, 0.0f, 1.0f, true, offsetxLocation, offsetyLocation));
    images.push_back(parallax_image("./textures/layers/parallax-mountain-mountain-far.png", 0.01f, 0.005f, 1.0f, true, offsetxLocation, offsetyLocation));
    images.push_back(parallax_image("./textures/layers/parallax-mountain-mountains.png", 0.1f, 0.05f, 2.0f, false, offsetxLocation, offsetyLocation));
    images.push_back(parallax_image("./textures/layers/parallax-mountain-trees.png", 0.5f, 0.25f, 2.0f, false, offsetxLocation, offsetyLocation));
    images.push_back(parallax_image("./textures/layers/parallax-mountain-foreground-trees.png", 1.0f, 1.0f, 2.0f, false, offsetxLocation, offsetyLocation));

    for (auto &image : images)
    {
        image.bind();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        // position attribute
        glBindBuffer(GL_ARRAY_BUFFER, backgroundVBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // texture attribute
        unsigned int textureVBO;
        glGenBuffers(1, &textureVBO);
        glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
        float buffer[8];
        image.fillTextureBuffer(buffer, LENGTH / 2, HEIGHT / 2);
        glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(1);
    }

    sprite redPlaneSprite("./textures/red-plane-sprite.png", 5, 5, offsetxLocation, offsetyLocation);

    unsigned int planeVAO;
    glGenVertexArrays(1, &planeVAO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // position attribute
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // texture attribute
    float buffer[8];
    redPlaneSprite.FillTextureBuffer(buffer);
    unsigned int textureVBO;
    glGenBuffers(1, &textureVBO);
    glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);

    float speed[] = {0.002, 0.002};
    game_entity planes[] = {game_entity(0, 0, -0.5, 0, 0), game_entity(0.2, 0, -0.5, 0, 0)};
    vector2d screenCenter[] = {{-0.5f, 0}, {0.5f, 0}};
    timer t(FPS_MAX);
    while (!glfwWindowShouldClose(window))
    {
        t.update();
        processInput(window, planes);
        for (int i = 0; i < NUMBER_OF_PLANES; i++)
        {
            planes[i].step(t.getElapsedTime());
            if (planes[i].position.y < 0)
                planes[i].position.y = 0;
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (parallax_image image : images)
        {
            for (int i = 0; i < NUMBER_OF_PLANES; i++)
            {
                image.bind();
                image.scrollTo(planes[i].position);
                glUniform1f(positionxLocation, screenCenter[i].x);
                glUniform1f(positionyLocation, 0.0f);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
        }

        glBindVertexArray(planeVAO);
        for (int i = 0; i < NUMBER_OF_PLANES; i++)
        {
            for (int j = 0; j < NUMBER_OF_PLANES; j++)
            {
                glUniform1f(positionxLocation, screenCenter[i].x + planes[j].position.x - planes[i].position.x);
                glUniform1f(positionyLocation, screenCenter[i].y + planes[j].position.y - planes[i].position.y);
                redPlaneSprite.BindAction(actionFromAngle(planes[j].angle));
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window, game_entity planes[])
{
    planes[0].angularVelocity = 0;
    planes[1].angularVelocity = 0;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        planes[0].angularVelocity = -ANGULAR_VELOCITY;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        planes[0].angularVelocity = ANGULAR_VELOCITY;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        planes[1].angularVelocity = -ANGULAR_VELOCITY;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        planes[1].angularVelocity = ANGULAR_VELOCITY;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    for (auto image : images)
    {
        image.bind();
        unsigned int textureVBO;
        glGenBuffers(1, &textureVBO);
        glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
        float buffer[8];
        image.fillTextureBuffer(buffer, width / 2, height / 2);
        glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(1);
    }
}

unsigned int actionFromAngle(float angle)
{
    //red plane actions should go from 0 to 24 as the planes turns from 0 to 2*PI radians
    unsigned int result = mod((int)floor((angle / (3.1415f * 2)) * 24), 24);
    return result == 25 ? 0 : result;
}

unsigned int mod(int a, int b)
{
    return (b + (a % b)) % b;
}