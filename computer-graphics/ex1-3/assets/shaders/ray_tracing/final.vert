#version 330 core
layout(location = 1) in vec2 aPos;
layout(location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos, 0.99, 1.0);
    TexCoord = aTexCoord;
}
