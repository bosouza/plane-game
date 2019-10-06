#include <bits/stdc++.h>
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#define WIDTH 1000
#define HEIGHT 500
#define NUMBER_OF_PLANES 2
#define ANGULAR_VELOCITY 2
#define FPS_MAX 120
#define BULLET_TRAVEL_DISTANCE 5
#define BULLET_SPEED 3
#define EXPLOSION_FRAME_PERIOD 0.03

#include <util_opengl.h>
#include <button_util.h>
#include <parallax_image.h>
#include <sprite.h>
#include <game_entity.h>
#include <timer.h>
#include <bullet.h>
#include <explosion.h>

using namespace std;

typedef struct
{
    pair<int, int> lowerLeftCorner;
    int width;
    int height;
} viewport;

unsigned int mod(int a, int b);
unsigned int actionFromAngle(float angle);
void processInput(GLFWwindow *window, game_entity planes[]);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void bindViewport(viewport);
unsigned int createVAO(unsigned int *VAO, unsigned int VBO, unsigned int EBO, sprite &s);

viewport viewports[] = {
    {
        lowerLeftCorner : {0, 0},
        width : WIDTH / 2,
        height : HEIGHT,
    },
    {
        lowerLeftCorner : {(WIDTH / 2) + 1, 0},
        width : WIDTH / 2,
        height : HEIGHT,
    }};

vector<parallax_image> images;
press_event buttons[NUMBER_OF_PLANES];

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

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Simple translation exercise", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

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
    fillRectangleBuffer(2, 2, backgroundBuffer);
    float planeBuffer[12];
    fillRectangleBuffer(0.15f, 0.2f, planeBuffer);
    float bulletBuffer[12];
    fillRectangleBuffer(0.02f, 0.03f, bulletBuffer);
    float explosionBuffer[12];
    fillRectangleBuffer(1, 1, explosionBuffer);

    unsigned int backgroundVBO;
    glGenBuffers(1, &backgroundVBO);
    glBindBuffer(GL_ARRAY_BUFFER, backgroundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundBuffer), backgroundBuffer, GL_STATIC_DRAW);

    unsigned int planeVBO;
    glGenBuffers(1, &planeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeBuffer), planeBuffer, GL_STATIC_DRAW);

    unsigned int bulletVBO;
    glGenBuffers(1, &bulletVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bulletVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bulletBuffer), bulletBuffer, GL_STATIC_DRAW);

    unsigned int explosionVBO;
    glGenBuffers(1, &explosionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, explosionVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(explosionBuffer), explosionBuffer, GL_STATIC_DRAW);

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
    images.push_back(parallax_image("./textures/layers/parallax-mountain-mountain-far.png", 0.005f, 0.005f, 1.0f, true, offsetxLocation, offsetyLocation));
    images.push_back(parallax_image("./textures/layers/parallax-mountain-mountains.png", 0.01f, 0.01f, 0.7f, false, offsetxLocation, offsetyLocation));
    images.push_back(parallax_image("./textures/layers/parallax-mountain-trees.png", 0.05f, 0.05f, 0.4f, false, offsetxLocation, offsetyLocation));
    images.push_back(parallax_image("./textures/layers/parallax-mountain-foreground-trees.png", 0.1f, 0.1f, 0.2f, false, offsetxLocation, offsetyLocation));

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
        image.fillTextureBuffer(buffer, WIDTH / 2, HEIGHT);
        glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(1);
    }

    //plane VAO
    sprite redPlaneSprite("./textures/red-plane-sprite.png", 5, 5, offsetxLocation, offsetyLocation);
    unsigned int planeVAO;
    createVAO(&planeVAO, planeVBO, EBO, redPlaneSprite);

    //bullet VAO
    sprite bulletSprite("./textures/bullet.png", 1, 1, offsetxLocation, offsetyLocation);
    unsigned int bulletVAO;
    createVAO(&bulletVAO, bulletVBO, EBO, bulletSprite);

    //explosion VAO
    sprite explosionBigSprite("./textures/explosion-nuclear.png", 5, 5, offsetxLocation, offsetyLocation);
    unsigned int explosionVAO;
    createVAO(&explosionVAO, explosionVBO, EBO, explosionBigSprite);

    game_entity planes[] = {game_entity(0, 0, 1, 0, 0), game_entity(0.2, 0, 1, 0, 0)};
    vector2d screenPosition[NUMBER_OF_PLANES];
    timer t(FPS_MAX);
    list<bullet> bullets;
    list<explosion> explosions;
    while (!glfwWindowShouldClose(window))
    {
        t.update();
        processInput(window, planes);
        for (int i = 0; i < NUMBER_OF_PLANES; i++)
        {
            if (!planes[i].visible)
                continue;
            planes[i].step(t.getElapsedTime());
            if (planes[i].position.y < -0.99)
            {
                planes[i].speed = 0;
                planes[i].visible = false;
                explosions.push_back(explosion({planes[i].position.x, planes[i].position.y + 0.4f}, &explosionBigSprite, EXPLOSION_FRAME_PERIOD));
            }
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //draw background images
        for (parallax_image image : images)
        {
            for (int i = 0; i < NUMBER_OF_PLANES; i++)
            {
                image.bind();
                screenPosition[i] = planes[i].position;
                if (screenPosition[i].y < 0)
                    screenPosition[i].y = 0;
                image.scrollTo(screenPosition[i]);
                bindViewport(viewports[i]);
                glUniform1f(positionxLocation, 0.0f);
                glUniform1f(positionyLocation, 0.0f);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
        }

        //draw planes
        glBindVertexArray(planeVAO);
        for (int i = 0; i < NUMBER_OF_PLANES; i++)
        {
            bindViewport(viewports[i]);
            for (int j = 0; j < NUMBER_OF_PLANES; j++)
            {
                if (!planes[j].visible)
                    continue;
                glUniform1f(positionxLocation, planes[j].position.x - screenPosition[i].x);
                glUniform1f(positionyLocation, planes[j].position.y - screenPosition[i].y);
                redPlaneSprite.BindAction(actionFromAngle(planes[j].angle));
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
        }

        // add new bullets
        for (int i = 0; i < NUMBER_OF_PLANES; i++)
        {
            if (buttons[i].pressEvent())
            {
                if (!planes[i].visible)
                    continue;
                bullets.push_back(bullet(planes[i].position.x, planes[i].position.y, BULLET_SPEED, planes[i].angle, 0, i, BULLET_TRAVEL_DISTANCE));
            }
        }

        //draw bullets
        glBindVertexArray(bulletVAO);
        bulletSprite.BindAction(0);
        for (int i = 0; i < NUMBER_OF_PLANES; i++)
        {
            bindViewport(viewports[i]);
            for (auto bulletI = bullets.begin(); bulletI != bullets.end(); bulletI++)
            {
                if (!bulletI->isAlive())
                {
                    bullets.erase(bulletI);
                    continue;
                }
                bulletI->step(t.getElapsedTime());
                glUniform1f(positionxLocation, bulletI->position.x - screenPosition[i].x);
                glUniform1f(positionyLocation, bulletI->position.y - screenPosition[i].y);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
        }

        //draw explosions
        glBindVertexArray(explosionVAO);
        for (int i = 0; i < NUMBER_OF_PLANES; i++)
        {
            bindViewport(viewports[i]);
            for (auto explosionI = explosions.begin(); explosionI != explosions.end(); explosionI++)
            {
                if (explosionI->updateAndBind())
                {
                    explosions.erase(explosionI);
                }
                glUniform1f(positionxLocation, explosionI->position.x - screenPosition[i].x);
                glUniform1f(positionyLocation, explosionI->position.y - screenPosition[i].y);
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

    buttons[0].setState(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
    buttons[1].setState(glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    viewports[0] = viewport{
        lowerLeftCorner : {0, 0},
        width : width / 2,
        height : height
    };
    viewports[1] = viewport{
        lowerLeftCorner : {(width / 2) + 1, 0},
        width : width / 2,
        height : height
    };
    for (auto image : images)
    {
        image.bind();
        unsigned int textureVBO;
        glGenBuffers(1, &textureVBO);
        glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
        float buffer[8];
        image.fillTextureBuffer(buffer, width / 2, height);
        glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(1);
    }
}

unsigned int createVAO(unsigned int *VAO, unsigned int VBO, unsigned int EBO, sprite &s)
{
    glGenVertexArrays(1, VAO);
    glBindVertexArray(*VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // position attribute
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // texture attribute
    float buffer[8];
    s.FillTextureBuffer(buffer);
    unsigned int textureVBO;
    glGenBuffers(1, &textureVBO);
    glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
}

void bindViewport(viewport v)
{
    glViewport(v.lowerLeftCorner.first, v.lowerLeftCorner.second, v.width, v.height);
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