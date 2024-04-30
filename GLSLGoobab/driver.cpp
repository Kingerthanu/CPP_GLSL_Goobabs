#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <chrono>
#include <thread>
#include <random>


static const float _RESOLUTION[2] = {1800, 1800};
static float _CENTRIOD[2] = { 0.0f, 0.0f };




// Define cursor position callback function
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    // Normalize cursor position to the range [-1, 1]
    _CENTRIOD[0] = (float)(2.0 * xpos / _RESOLUTION[0] - 1.0);
    _CENTRIOD[1] = (float)(1.0 - 2.0 * ypos / _RESOLUTION[1]);

    // Update uniform variable representing cursor position
    //glUniform2fv(U_CENTER, 1, &_CENTRIOD[0]);



}


int main()
{

    // Initialize GLFW and create the main window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* _WINDOW = glfwCreateWindow(_RESOLUTION[0], _RESOLUTION[1], "Goobab", NULL, NULL);

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


   

    int U_RESOLUTION  = glGetUniformLocation(shaderProcess.getID(), "resolution");
    int U_TIMER = glGetUniformLocation(shaderProcess.getID(), "time");
    //U_CENTER = glGetUniformLocation(shaderProcess.getID(), "u_mouse");

    glUniform2fv(U_RESOLUTION, 1, &_RESOLUTION[0]);
    //glUniform2fv(U_CENTER, 1, &_CENTRIOD[0]);
 

    std::chrono::duration<double> frameDuration(1.0 / 60);

    //std::cout << U_RESOLUTION << ' ' << U_CENTER  << '\n';

    float tick = 1.0f;

    glfwSetCursorPosCallback(_WINDOW, cursor_position_callback);


    while (!glfwWindowShouldClose(_WINDOW)) {

        auto start = std::chrono::high_resolution_clock::now();

        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1f(U_TIMER, tick);
        tick += 0.001271f;
        if (2.5f <= tick && tick <= 3.5f) {
            tick = 3.71f;
        }
        else if (6.0f <= tick && tick <= 7.0f) {
            tick = 7.05f;
        }
        else if (tick >= 7.3f) {
            tick = 1.0f;
        }
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

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