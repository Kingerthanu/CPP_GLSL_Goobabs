precision highp float;

uniform vec2 resolution;
uniform float time;

const int MAX_ITER = 400;
const float BAIL_OUT = 2.78;

void main() {
    vec2 uv = (gl_FragCoord.xy - 0.5 * resolution.xy) / resolution.y;
    vec2 c = vec2(-0.91289, -0.386 * cos(time));
    vec2 z = uv * 3.0;

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
