#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <chrono>
#include <thread>
#include <random>


// Essential Globals
static const float _RESOLUTION[2] = {1800, 1800};               // Our Resolution Will Be A Constant Throughout
static float _CENTRIOD[2] = { 0.0f, 0.0f };                     // Center For The Viewport To Zoom Into For The Mandelbrot
float tick = 1.0f;                                              // Time Tick For .frag To Simulate Movement
float tickStep = 0.0f;                                          // How Fast To Tick (Basically Slow-Mo Or Fast-Forward)
float zoomStep = 0.0f;                                          // How Fast To Zoom (Don't Zoom Initially)
bool mousePressed = false;                                      // Detect Left-Mouse-Button Pressing For Centering Viewport



void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
   
    // If The User Has Pressed Left Mouse Button
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            mousePressed = true;
        }
        else if (action == GLFW_RELEASE) {
            mousePressed = false;
        }
    }
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    // If The User Presses Enter, Move Forward A Time Increment
    if (key == GLFW_KEY_ENTER && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        tick += tickStep;
    }
    // If The User Presses Up-Arrow, Increase The Speed Of Our Time
    else if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        tickStep += 0.0001f;
        std::cout << tickStep << '\n';
    }
    // If The User Presses Down-Arrow, Decrease The Speed Of Our Time
    else if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        tickStep -= 0.0001f;
        std::cout << tickStep << '\n';
    }
    // If The User Presses Left-Arrow, Increase The Speed Of Our Zoom
    else if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        zoomStep += 0.0001f;
    }
    // If The User Presses Right-Arrow, Decrease The Speed Of Our Zoom
    else if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        zoomStep -= 0.0001f;
    }

}

// Define cursor position callback function
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {

    // Only Update The Center To Zoom Into Only When The User Presses Left-Click (More Optimization Then A Need)
    if(mousePressed){
        _CENTRIOD[0] = (float)(2.0 * xpos / _RESOLUTION[0] - 1.0);
        _CENTRIOD[1] = (float)(1.0 - 2.0 * ypos / _RESOLUTION[1]);
    }



}


int main()
{

    // Initialize GLFW and create the main window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create Window
    GLFWwindow* _WINDOW = glfwCreateWindow(_RESOLUTION[0], _RESOLUTION[1], "Goobab", NULL, NULL);

    // Ensure Creation
    if (!_WINDOW) {
        glfwTerminate();
        return -1;
    }
    
    // Contexualize This Window As Active
    glfwMakeContextCurrent(_WINDOW);
    gladLoadGL();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


    // Spark Our Shader
    Shader shaderProcess("main.vert", "main.frag");
    shaderProcess.Activate();
    
    // Set Up Our Canvas (Full Window So Just Simple Quad)
    GLfloat vertices[] = {
        -1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f,
         1.0f, -1.0f

    };

    // Spark VBOs And VAOs (Lazy Definition Only Using Register)
    GLuint VBO, VAO;

    // Grab Memory For VBO and VAO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Connect VBO With VAO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Give Context For VBO
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Grab Our Needed Uniforms (Registers In .frag)
    int U_TIMER = glGetUniformLocation(shaderProcess.getID(), "time");
    int U_CENTER = glGetUniformLocation(shaderProcess.getID(), "center");
    int U_ZOOM = glGetUniformLocation(shaderProcess.getID(), "zoom");

    // Only Will Be Called Once So Don't Need To Store Uniform Register
    glUniform2fv(glGetUniformLocation(shaderProcess.getID(), "resolution"), 1, &_RESOLUTION[0]);

    // Set 60fps
    std::chrono::duration<double> frameDuration(1.0 / 60);

    // Set The Interrupt Handlers Of Our Window
    glfwSetCursorPosCallback(_WINDOW, cursor_position_callback);
    glfwSetKeyCallback(_WINDOW, key_callback);
    glfwSetMouseButtonCallback(_WINDOW, mouse_button_callback);

    // Our Inital Zoom
    float zoom = 4.0f;

    while (!glfwWindowShouldClose(_WINDOW)) {

        auto start = std::chrono::high_resolution_clock::now();

        glClear(GL_COLOR_BUFFER_BIT);

        // Pass In Our Values To The GPU
        glUniform1f(U_TIMER, tick);
        glUniform1f(U_ZOOM, zoom);
        
        // If We Have Fallen Into The Range In Which Will Give Us A Long Stupor Of Darkness; Skip Over It So User Doesn't Wait
        if (2.5f <= tick && tick <= 3.5f) {
            tick = 3.71f;
        }
        else if (6.0f <= tick && tick <= 7.0f) {
            tick = 7.05f;
        }
        else if (tick >= 7.3f) {
            tick = 1.0f;
        }

        // If The User Has Pressed The Left-Mouse Button, Update Our Uniform For The Viewport Center To Zoom Into
        if (mousePressed) {
            glUniform2fv(U_CENTER, 1, &_CENTRIOD[0]);
        }

        // Increment Our Zoom Every Main Cycle (A Little Unoptimized As We Could Be += 0.0f)
         zoom += zoomStep;

        
        // After Done, Now Tell GL To Render
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glfwSwapBuffers(_WINDOW);
        glfwPollEvents();

        // Check Current Cycle-Time And Wait
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

        // Sleep to maintain the desired frame rate
        std::this_thread::sleep_for(frameDuration - elapsed);

    }
	

    // Cleanup :D
    shaderProcess.Delete();
    glfwDestroyWindow(_WINDOW);
    glfwTerminate();


    return 0;

};