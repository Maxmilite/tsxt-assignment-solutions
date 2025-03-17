#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTex;

vec3 toneMap(vec3 color) {
    return color / (color + vec3(1.0));
}

void main() {
    vec4 final = texture(screenTex, TexCoord);
    final.rgb = toneMap(final.rgb);

    FragColor = vec4(final.rgb, 1.0);

}
