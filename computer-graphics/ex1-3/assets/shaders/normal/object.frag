#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

void main() {

	vec3 norm = normalize(Normal);

	vec3 result = norm;

	FragColor = vec4(result, 1.0);
}