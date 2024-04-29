#version 330 core

// Input vertex attributes
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;

void main()
{

    // Set the position of the vertex
    gl_Position = vec4(aPos, 0.0f, 1.0);

}
