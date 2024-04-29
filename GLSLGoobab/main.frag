#version 330 core

uniform vec2 _center;       // Center position of the circle
uniform float _radius;      // Radius of the circle
uniform float _rotationCos; // Precomputed cosine of rotation angle
uniform float _rotationSin; // Precomputed sine of rotation angle
uniform float _breath;

out vec4 fragColor; // Output color of the fragment

void main()
{
     // Normalize fragment coordinates between -1 and 1
    vec2 normalizedFragCoord = (2.0 * gl_FragCoord.xy - vec2(1000.0)) / vec2(1000.0);
    
    // Calculate the distance from the fragment's position to the center of the circle
    vec2 distance = normalizedFragCoord - _center;
    float distanceToCenter = length(distance);
    
    // If the fragment is outside the circle, discard it
    if (distanceToCenter > _radius) {
        discard;
    }
    
    // Calculate the angle of the fragment relative to the center of the circle
    float angle = atan(distance.y, distance.x) * _breath;
    
    // Calculate the displacement of the fragment based on the angle and time
    float displacement = sin(angle * 731.0 - _breath) * 7.937;
    
    // Displace the fragment along the normal vector of the circle
    vec2 displacedPosition = normalizedFragCoord + normalize(distance) * displacement;
    
    // If the displaced fragment is inside the circle, set the fragment color to red
    // Otherwise, set the fragment color to black
    float displacedDistanceToCenter = length(displacedPosition - _center);
    fragColor = (displacedDistanceToCenter <= _radius) ? vec4(0.6, 0.5, distanceToCenter, 1.0) : vec4(0.0, 0.0, 0.0, 1.0);


}
