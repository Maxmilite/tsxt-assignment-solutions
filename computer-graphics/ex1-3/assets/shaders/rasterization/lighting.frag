#version 330 core

out vec4 fragColor;

uniform vec3 lightPosition;
uniform float lightRadius;
uniform vec3 lightColor;

uniform vec2 screenSize;

uniform mat4 view;
uniform mat4 projection;

void main() {
    vec2 fragCoord = gl_FragCoord.xy;

    vec4 lightClipSpace = projection * view * vec4(lightPosition, 1.0);
    vec2 lightScreenPos = lightClipSpace.xy / lightClipSpace.w;
    lightScreenPos = lightScreenPos * 0.5 + 0.5; 
    lightScreenPos *= screenSize;

    float distance = length(fragCoord - lightScreenPos);

    if (distance <= lightRadius / max(1.0, lightClipSpace.w)) {
        float attenuation = 1.0 - (distance / lightRadius * max(1.0, lightClipSpace.w));
        attenuation = pow(attenuation, 9.0);
        fragColor = vec4(lightColor, attenuation);
    } else {
        discard;
    }
}
