#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <chrono>
#include <thread>
#include <random>


static const unsigned int WIDTH = 1000, HEIGHT = 1000;

float cntr = 1.5f;
float centriod[2] = {0.0f, 0.0f};
float distort = 0.0f;
float breath = 0.0f;

int main()
{

    // Initialize GLFW and create the main window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* _WINDOW = glfwCreateWindow(WIDTH, HEIGHT, "Goobab", NULL, NULL);

    if (!_WINDOW) {
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(_WINDOW);
    gladLoadGL();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


    // Activate shader program and other OpenGL resources
    Shader shaderProcess("main.vert", "main.frag");
    shaderProcess.Activate();
    
    // Set up vertex buffer and array for a quad
    GLfloat vertices[] = {
        -1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f,
         1.0f, -1.0f

    };

    // Light VBOs and VAOs.
    GLuint VBO, VAO;

    // Grab mem from opengl for VBO and VAO.
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Connect VBO with VAO.
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Give context for VBO.
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);


   

    int U_CENTER = glGetUniformLocation(shaderProcess.getID(), "_center");
    int U_RADIUS = glGetUniformLocation(shaderProcess.getID(), "_radius");
    int U_AMPLITUDE = glGetUniformLocation(shaderProcess.getID(), "_amplitude");
    int U_FREQUENCY = glGetUniformLocation(shaderProcess.getID(), "_frequency");
    //int U_COS = glGetUniformLocation(shaderProcess.getID(), "_rotationCos");
    //int U_SINE = glGetUniformLocation(shaderProcess.getID(), "_rotationSin");
    // int U_BREATH = glGetUniformLocation(shaderProcess.getID(), "_breath");
    int U_DISTORT = glGetUniformLocation(shaderProcess.getID(), "_distort");

    glUniform1f(U_AMPLITUDE, 20.0f);

    std::chrono::duration<double> frameDuration(1.0 / 60);

    while (!glfwWindowShouldClose(_WINDOW)) {

        auto start = std::chrono::high_resolution_clock::now();

        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1f(U_RADIUS, cntr);
        glUniform2fv(U_CENTER, 1, &centriod[0]);
        glUniform1f(U_DISTORT, distort);
        
       // glUniform1f(U_COS, std::cos(angle));
       // glUniform1f(U_SINE, std::sin(angle));

        glUniform1f(U_FREQUENCY, breath);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        cntr += 0.001f;
        breath += 0.5f;
        

        distort += 0.00371f;

        glfwSwapBuffers(_WINDOW);
        glfwPollEvents();

        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

        // Sleep to maintain the desired frame rate
        std::this_thread::sleep_for(frameDuration - elapsed);

    }
	

    shaderProcess.Delete();

    glfwDestroyWindow(_WINDOW);
    glfwTerminate();

    return 0;

};