#version 330 core

uniform vec2 resolution;
uniform vec2 center;
uniform float time;
uniform float zoom;

// Max Detail Of Our Mandelbrot
const int MAX_ITER = 800;

// How Far Out The Mandels Can Bloom
const float BAIL_OUT = 1.78;

void main() {

    // Relative Texture Coordinate Of This Given Pixel
    vec2 uv = (gl_FragCoord.xy - 0.5 * resolution.xy) / resolution.y;

    // Adjust UV Coordinates Based On The Zoom And Center
    uv = uv * zoom - center;

    // Calculate Julias Constants, c, Based On Time
    vec2 c = vec2(-0.2989 * sin(time), 0.386 * cos(time));

    // Initialize z As The Current UV Coordinates
    vec2 z = uv;

    // Apply Transformation To Emphasize Straight Edges And Corners
    z = abs(z);
    z = vec2(z.x * z.x * z.x - 3.0 * z.x * z.y * z.y, 3.0 * z.x * z.x * z.y - z.y * z.y * z.y);

    // Help Proportionalize Our Given Textures
    float aspectRatio = resolution.x / resolution.y;
    z.x *= aspectRatio;

    // Main Mandelbrot Meat
    int iter;
    for (iter = 0; iter < MAX_ITER; iter++) {
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
        if (dot(z, z) > BAIL_OUT * BAIL_OUT) break;
    }

    // Get The Color Based Upon How Far Our Coordinate Has "Ran" From The Mandel
    float color = float(iter) / float(MAX_ITER);
    gl_FragColor = vec4(color * 0.76, color * 0.3, 0.0, 1.0);
}
