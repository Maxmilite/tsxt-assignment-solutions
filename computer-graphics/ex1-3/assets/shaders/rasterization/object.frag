#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 viewPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material0, material1, material2, material3;
uniform Light light;

void main() {
	vec3 textureColor = vec3(texture(material0.diffuse, TexCoord));
	
	// ambient
	vec3 ambient = light.ambient * textureColor;

	// diffuse
	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * textureColor;

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material0.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material0.specular, TexCoord));  

	vec3 result = ambient + diffuse + specular;

	FragColor = vec4(result, 1.0);
}