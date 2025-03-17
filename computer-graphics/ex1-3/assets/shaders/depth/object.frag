#version 330 core
out vec4 FragColor;

void main() {
    float depth = gl_FragCoord.z;
    depth = 1.0 - depth;
    FragColor = vec4(vec3(depth), 1.0);
}