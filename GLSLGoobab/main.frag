precision highp float;

uniform vec2 resolution;
uniform vec2 center;
uniform float time;
uniform float zoom;

const int MAX_ITER = 800;
const float BAIL_OUT = 1.78;

void main() {
    vec2 uv = (gl_FragCoord.xy - 0.5 * resolution.xy) / resolution.y;

    // Adjust UV coordinates based on the zoom and center
    uv = uv * zoom - center;

    // Calculate complex constant c based on time
    vec2 c = vec2(-0.91289, -0.386 * cos(time));

    // Initialize z as the current UV coordinates
    vec2 z = uv;

    // Apply transformation to emphasize straight edges and corners
    z = abs(z);
    z = vec2(z.x * z.x * z.x - 3.0 * z.x * z.y * z.y, 3.0 * z.x * z.x * z.y - z.y * z.y * z.y);

    float aspectRatio = resolution.x / resolution.y;
    z.x *= aspectRatio;

    int iter;
    for (iter = 0; iter < MAX_ITER; iter++) {
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
        if (dot(z, z) > BAIL_OUT * BAIL_OUT) break;
    }

    float color = float(iter) / float(MAX_ITER);
    gl_FragColor = vec4(color * 0.76, color * 0.3, 0.0, 1.0);
}
