#version 330 core

uniform vec2 _center;       // Center position of the circle
uniform float _radius;      // Radius of the circle
uniform float _amplitude;   // Controls the intensity of the distortion
uniform float _frequency;   // Controls the frequency of the distortion
uniform float _distort;

out vec4 fragColor;         // Output color of the fragment

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
    float angle = atan(distance.y, distance.x);
    
    // Calculate the displacement of the fragment based on the angle and frequency
    float displacement = _amplitude * sin(angle * _frequency);
    
    // Displace the fragment along the normal vector of the circle
    vec2 displacedPosition = normalizedFragCoord + normalize(distance) * displacement;
    
    // If the displaced fragment is inside the circle, set the fragment color to red
    // Otherwise, set the fragment color to black
    float displacedDistanceToCenter = length(displacedPosition - _center);
    fragColor = (displacedDistanceToCenter <= _radius) ? vec4(cos(distanceToCenter), 0.0, 0.0, 1.0) : vec4(0.0, 0.0, 0.0, 1.0);
}
